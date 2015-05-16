// TODO
//  add exceptions where noted
//  write unit tests

#include "game_board.h"
#include <fstream>
using std::ifstream;

#define ALIVE '#'
#define DEAD '.'


GameBoard::GameBoard(int width, int height)
{
    this->width = width;
    this->height = height;
    this->grid = create_grid_array(width, height);
    this->buffer = create_grid_array(width, height);
}

GameBoard::~GameBoard()
{
    for (int i = 0; i < this->width; ++i) {
        delete this->grid[i];
        delete this->buffer[i];
    }
    delete this->grid;
    delete this->buffer;
}

bool** GameBoard::create_grid_array(int width; int height)
{
    bool** grid_array_pointer = new bool*[height];
    for (int i = 0; i < height; ++i)
        grid_array_pointer[i] = new bool[width];
    return grid_array_pointer;
}

// swap main grid with buffer and clear buffer
// Yes, this is necessary. std::swap caused problems in the game loop.
void buffer_swap()
{
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this-> width; ++j) {
            this->grid[i][j] = this->buffer[i][j];
            buffer[i][j] = false;
        }
    }
}

// simulate one turn in the Game of Life
void GameBoard::step()
{
    int live_neighbors;

    for (int y = 0; y < this->height; ++y) {
        for (int x = 0; x < this->width; ++x) {
            live_neighbors = this->count_live_neighbors(x, y);

            // live cell with fewer than two neighbors dies (under-population)
            if (this->grid[y][x] && live_neighbors < 2)
                buffer[y][x] = false;

            // live cell with two or three live neighbors lives on to next generation
            else if (this->grid[y][x] && (live_neighbors == 2 || live_neighbors == 3))
                buffer[y][x] = true;

            // live cell with more than three live neighbors dies (over-population)
            else if (this->grid[y][x] && live_neighbors > 3)
                buffer[y][x] = false;

            else if (!this->grid[y][x] && live_neighbors == 3)
                buffer[y][x] = true;
        }
    }
    this->buffer_swap();
}

// read a text file and modify grid to match
void GameBoard::set_state(string filename)
{
    ifstream init_file(filename);
    // RAISE FILE_NOT_FOUND_ERROR IF FILE DOES NOT EXIST
    string line;

    while (std::getline(init_file, line)) {
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                if (line[j] == ALIVE)
                    this->grid[i][j] = true;
                else if (line[j] == DEAD)
                    this->grid[i][j] = false;
                else
                    // RAISE INVALID_INPUT_ERROR
            }
        }
    }

    init_file.close();
}

// returns number of living cells adjacent to given point
int GameBoard::count_live_neighbors(int target_x, int target_y)
{
    int live_neighbors = 0;

    for (int y = target_y - 1; y <= target_y + 1; ++y) {
        for (int x = target_x - 1; x <= target_x + 1; ++x) {
            if (y == target_y && x == target_x) // ignore target point
                continue;
            // add length of row/column to offset negative integer in mod calculation
            if (this->grid[(y + this->height) % this->height][(x + this->width) % this->width])
                ++live_neighbors;
        }
    }
    return live_neighbors;
}