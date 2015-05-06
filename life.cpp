#include <ncurses.h>
#include <fstream>
using std::ifstream;
#include <string>
using std::string; using std::getline;
#include <utility>
using std::swap;
#include <thread>
using std::this_thread::sleep_for;
#include <chrono>
using std::chrono::seconds;

#define TEST_FILE "init_samples/glider.txt"
#define ALIVE '#'
#define DEAD '.'


// create 2D array to use as the grid
bool** createGridArray(int grid_width, int grid_height)
{
    bool** grid_array_pointer = new bool*[grid_height];
    for (int i = 0; i < grid_height; ++i) {
        grid_array_pointer[i] = new bool[grid_width];
    }
    return grid_array_pointer;
}


// set initial state of the grid
// ADD AN EXCEPTION HERE
void setInitialState(bool** grid, ifstream& init_file, int grid_width, int grid_height)
{
    // get a line to represent each row and copy it into the grid
    string row;
    for (int y = 0; y < grid_height; ++y) {
        getline(init_file,row);
        for (int x = 0; x < grid_width; ++x) {
            if (row[x] == ALIVE) {
                grid[y][x] = true;
            }
            else if (row[x] == DEAD) {
                grid[y][x] = false;
            }
            else {
                // raise not_implemented_error;
            }
        }
    }
}


// Sum the values of each neighboring cell in the grid and return it
// Since the grid is a bool array, should be equal to number of live cells
int countNeighbors(bool** grid, int grid_width, int grid_height, int target_y, int target_x)
{
    int live_neighbors = 0;
    for (int y = target_y - 1; y <= target_y + 1; ++y) {
        for (int x = target_x - 1; x <= target_x + 1; ++x) {
            if (y == target_y && x == target_x) // ignore target itself
                continue;
            // add length of row/column to offset effect of negative int in mod calculation
            if (grid[(y + grid_height) % grid_height][(x + grid_width) % grid_width])
                ++live_neighbors;
        }
    }
    return live_neighbors;
}


void step(bool** grid, bool** buffer, int grid_width, int grid_height)
{
    int live_neighbors;

    for (int y = 0; y < grid_height; ++y) {
        for (int x = 0; x < grid_width; ++x) {
            live_neighbors = countNeighbors(grid, grid_width, grid_height, y, x);

            // live cell with fewer than two live neighbors dies (under-population)
            if (grid[y][x] && live_neighbors < 2 )
                buffer[y][x] = false;

            // live cell with two or three live neighbors lives on to next generation
            else if ((grid[y][x] && live_neighbors == 2) || (grid[y][x] && live_neighbors == 3))
                buffer[y][x] = true;

            // live cell with more than three neighbors dies (over-population)
            else if (grid[y][x] && live_neighbors > 3)
                buffer[y][x] = false;

            // dead cell with exactly three live neighbors becomes a live cell (reproduction)
            else if (!grid[y][x] && live_neighbors == 3)
                buffer[y][x] = true;
        }
    }
}


void printFrame(WINDOW* win, bool** grid)
{
    wclear(win);

    int h, w;
    getmaxyx(win, h, w);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j])
                mvwaddch(win, i, j, ALIVE);
        }
    }
    wrefresh(win);
}


WINDOW* createGameWindow(int height, int width, int starty, int startx)
{
    WINDOW* win;
    win = newwin(height, width, starty, startx);

    // box(win, 0, 0);
    wrefresh(win);

    return win;
}


int main()
{
    int turns, width, height;

    initscr();              // start curses mode
    clear();
    noecho();
    cbreak();               // disable line buffering
    keypad(stdscr, TRUE);   // get F keys
    curs_set(0);
    refresh();

    ifstream init_file;
    init_file.open(TEST_FILE);
    init_file >> turns >> width >> height;
    init_file.seekg(1, init_file.cur);

    bool** life_grid = createGridArray(width, height);
    bool** buffer = createGridArray(width, height);
    setInitialState(life_grid, init_file, width, height);
    init_file.close();

    int starty = (getmaxy(stdscr) - height) / 2;
    int startx = (getmaxx(stdscr) - width) / 2;


    WINDOW* game_window = createGameWindow(height, width, starty, startx);
    wrefresh(game_window);

    printFrame(game_window, life_grid);

    // main game loop
    for (int t = 0; t < turns; ++t) {
        step(life_grid, buffer, width, height);
        swap(life_grid, buffer);
        printFrame(game_window, life_grid);
        sleep_for(seconds(1));
    }

    delwin(game_window);
    endwin();   // end curses mode

    return 0;
}