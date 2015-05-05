#include <ncurses.h>
#include <fstream>
using std::ifstream;
#include <string>
using std::string; using std::getline;

#define TEST_FILE "init_samples/sample.txt"
#define ALIVE '#'
#define DEAD '.'


// create 2D array to use as the grid
bool** createGridArray(unsigned int grid_width, unsigned int grid_height)
{
    bool** grid_array_pointer = new bool*[grid_height];
    for (unsigned int i = 0; i < grid_height; ++i) {
        grid_array_pointer[i] = new bool[grid_width];
    }
    return grid_array_pointer;
}


// set initial state of the grid
// ADD AN EXCEPTION HERE
void setInitialState(bool** grid, ifstream& init_file, unsigned int grid_width, unsigned int grid_height)
{
    // get a line to represent each row and copy it into the grid
    string row;
    for (unsigned int y = 0; y < grid_height; ++y) {
        getline(init_file,row);
        for (unsigned int x = 0; x < grid_width; ++x) {
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

// just a test function
void showInitialState(WINDOW* win, bool** grid)
{
    int h, w;
    getmaxyx(win, h, w);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (grid[i][j])
                mvwaddch(win, i, j, ACS_BLOCK);
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
    setInitialState(life_grid, init_file, width, height);
    init_file.close();

    int starty = (getmaxy(stdscr) - height) / 2;
    int startx = (getmaxx(stdscr) - width) / 2;


    WINDOW* game_window = createGameWindow(height, width, starty, startx);
    wrefresh(game_window);
    showInitialState(game_window, life_grid);
    getch();

    endwin();   // end curses mode

    return 0;
}