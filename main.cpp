/* To-Do:

- add option to load initial state from a text file
- write rules for cells living/dying
- add checks for input size during setInitialState()
- add check for correct character values when initializing grid
- make grid into a toroidal array so cells loop when moving out of bounds
- add function to randomly generate an initial state, given grid dimensions
*/

#include <iostream>
using std::cin; using std::cout; using std::endl;
#include <string>
using std::string; using std::getline;


// create 2D array to use as the grid
// '#' are live cells and '.' are dead cells
char** createGridArray(int grid_width, int grid_depth)
{
    char** grid_array_pointer = new char*[grid_depth];
    for (unsigned int i = 0; i < grid_depth; ++i) {
        grid_array_pointer[i] = new char[grid_width];
    }
    return grid_array_pointer;
}


// set initial state of the grid
void setInitialState(char** grid, int grid_width, int grid_depth)
{
    // get a line to represent each row and copy it into the grid
    string row;
    for (unsigned int i = 0; i < grid_depth; ++i) {
        getline(cin,row);
        for (unsigned int j = 0; j < grid_width; ++j) {
            grid[i][j] = row[j];
        }
    }
}


int main()
{
    int grid_width, grid_depth;
    cout << "Enter dimensions of grid: " << endl;
    cin >> grid_width >> grid_depth;

    char** life_grid = createGridArray(grid_width,grid_depth);
    setInitialState(life_grid, grid_width, grid_depth);

    // print grid to shell
    // for (unsigned int i = 0; i < grid_depth; ++i) {
    //     for (unsigned int j = 0; j < grid_width; ++j) {
    //         cout << life_grid[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}