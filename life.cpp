#include <ncurses.h>
#include <fstream>
using std::ifstream;
#include <string>
using std::string; using std::getline;
#include <thread>
using std::this_thread::sleep_for;
#include <chrono>
using std::chrono::milliseconds;

#define INIT_FILE "init_samples/glider_gun.txt"
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
        }
    }
}

// return the number of neighboring cells that are alive
int countNeighbors(bool** grid, int grid_width, int grid_height, int target_y, int target_x)
{
    int live_neighbors = 0;

    for (int y = target_y - 1; y <= target_y + 1; ++y) {
        for (int x = target_x - 1; x <= target_x + 1; ++x) {
            if (y == target_y && x == target_x) // ignore target itself
                continue;
            // add length of row/column to offset effect of negative integer in mod calculation
            if (grid[(y + grid_height) % grid_height][(x + grid_width) % grid_width])
                ++live_neighbors;
        }
    }
    return live_neighbors;
}

// write buffer to displayed grid, then clear it
void bufferSwap(bool** grid, bool** buffer, int grid_width, int grid_height)
{
    for (int i = 0; i < grid_height; ++i) {
        for (int j = 0; j < grid_width; ++j) {
            grid[i][j] = buffer[i][j];
            buffer[i][j] = false;
        }
    }
}

// determine if cells live or die
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
    bufferSwap(grid, buffer, grid_width, grid_height);
}

void printFrame(WINDOW* win, bool** grid)
{
    int h, w;
    werase(win);
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
    wrefresh(win);

    return win;
}

int main()
{
    int turns, width, height;

    initscr();              // start curses mode
    noecho();
    cbreak();               // disable line buffering
    curs_set(0);

    ifstream init_file;
    init_file.open(INIT_FILE);
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

    // main game loop
    for (int t = 0; t < turns; ++t) {
        printFrame(game_window, life_grid);
        sleep_for(milliseconds(125));
        step(life_grid, buffer, width, height);
    }

    delwin(game_window);
    endwin();   // end curses mode

    return 0;
}