#include <string>
using std::string;

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

class GameBoard
{
private:
    int height;
    int width;
    bool** grid;
    bool** buffer;
    bool** create_grid_array(int width, int height);
public:
    // Constructor & Destructor
    GameBoard(int width, int height);
    ~GameBoard();

    // Member Functions
    void step();
    void buffer_swap();
    void set_state(string filename);            // change grid to match given file
    int count_live_neighbors(int target_x, int target_y);     // count number of live neighbors

    // Accessors
    int get_width()  { return width; }
    int get_height() { return height; }
    bool get_point(int x, int y) { return grid[y][x]; }
};


#endif // GAME_BOARD_H