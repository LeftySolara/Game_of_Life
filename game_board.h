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
    // Constructors & Destructor
    GameBoard(int width, int height);                           // create array full of false values
    GameBoard(int width, int height, const char* filename[]);   // create and initialize
    ~GameBoard();

    // Member Functions
    void step();
    void grid_swap();
    void set_state(const char* filename[]);     // change grid to match given file
    int count_live_neighbors(int x, int y);     // count number of live neighbors

    // Accessors
    int get_width()  { return width; }
    int get_height() { return height; }
    bool get_point(int x, int y) { return grid[y][x]; }
};


#endif // GAME_BOARD_H