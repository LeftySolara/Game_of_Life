/* To-Do:

- write rules for cells living/dying
- add checks for input size during setInitialState()
- add check for correct character values when initializing grid
- make grid into a toroidal array so cells loop when moving out of bounds
- add function to randomly generate an initial state, given grid dimensions
- implement exceptions where marked
*/

#define ALIVE '#'
#define DEAD '.'

#include <iostream>
using std::cin; using std::cout; using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string; using std::getline;


// create 2D array to use as the grid
bool** createGridArray(unsigned int grid_width, unsigned int grid_depth)
{
    bool** grid_array_pointer = new bool*[grid_depth];
    for (unsigned int i = 0; i < grid_depth; ++i) {
        grid_array_pointer[i] = new bool[grid_width];
    }
    return grid_array_pointer;
}


// set initial state of the grid
// ADD AN EXCEPTION HERE
void setInitialState(bool** grid, ifstream& init_file, unsigned int grid_width, unsigned int grid_depth)
{
    // get a line to represent each row and copy it into the grid
    string row;
    for (unsigned int y = 0; y < grid_depth; ++y) {
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


void display_grid(bool** grid, unsigned int grid_width, unsigned int grid_depth)
{
    for (unsigned int i = 0; i < grid_depth; ++i) {
        for (unsigned int j = 0; j < grid_width; ++j) {
            if (grid[i][j]) {
                cout << ALIVE << " ";
            }
            else if (!grid[i][j]) {
                cout << DEAD << " ";
            }
        }
        cout << endl;
    }
}


int main()
{
    unsigned int grid_width, grid_depth, turns;
    string file_name;
    ifstream init_file;

    cout << "Enter name of initialization file:" << endl;
    cin >> file_name;
    init_file.open(file_name);

    // get dimensions of grid
    init_file >> grid_width >> grid_depth >> turns;

    // move buffer position to next line
    init_file.seekg(1, init_file.cur);

    // create grid and set initial state
    bool** life_grid = createGridArray(grid_width,grid_depth);
    setInitialState(life_grid, init_file, grid_width, grid_depth);
    display_grid(life_grid, grid_width, grid_depth);

    init_file.close();

    return 0;
}
