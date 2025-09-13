#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

//  🖍️ Set up output color presets

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0;0m"

const int OFFSET = 0; // This should stay 0, except when reviewing coordinate
                      // output, i.e. first cell is (1,1) instead of (0,0).
using namespace std;

struct cell {
    int row;
    int col;
    bool visited = false;
};

struct Maze {
    vector<vector<char>> layout;
    cell start;
    cell end;
};

/*  **ReadMaze**
 *  PRE: A "maze" file comprised of 1s and 0s as input.  It is assumed that
 *       each row of the file will contain the same number of characters, to
 *       form a complete rectangular maze.
 *
 *  PST: Maze data is read into a Maze vector;
*/

Maze ReadMaze(string fileName) {
    Maze maze;
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening file " << fileName << endl;
        exit(1);
    }
    istringstream iSS;
    string line;
    char inputChar;

    vector<char> row;

    while (getline(file, line)) {
        iSS.clear();
        iSS.str(line);
        while (iSS >> inputChar) {
            row.push_back(inputChar);
        }
        maze.layout.push_back(row);
        row.clear();
    }
    file.close();
    return maze;
}

/*  ** ScanFirstRow (Helper Function for FormatMaze
 *
 *  PRE: Maze contains only a 1 or a 0 stored in each coordinate
 *  PST: Any 0s sre replaced with 'E' (for Entry/Exit)
*/

void scanFirstRow (Maze &maze) {
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < maze.layout[i].size(); ++j) {
            if (maze.layout[i][j] == '0') {
                maze.layout[i][j] = 'E';
            }
        }
    }
}

/*  ** ScanLastRow (Helper Function for FormatMaze
 *
 *  PRE: Maze contains only a 1 or a 0 stored in each coordinate
 *  PST: Any 0s sre replaced with 'E' (for Entry/Exit)
*/
void scanLastRow (Maze &maze) {
    for (int i = maze.layout.size() - 1; i < maze.layout.size(); ++i) {
        for (int j = 0; j < maze.layout[i].size(); ++j) {
            if (maze.layout[i][j] == '0') {
                maze.layout[i][j] = 'E';
            }
        }
    }
}

/*  ** ScanFirstColumn (Helper Function for FormatMaze
 *
 *  PRE: Maze contains only a 1 or a 0 stored in each coordinate
 *  PST: Any 0s sre replaced with 'E' (for Entry/Exit)
*/

void scanFirstColumn (Maze &maze) {
    for (int i = 1; i < maze.layout.size() -1; ++i) {
        for (int j = 0; j < 1; ++j) {
            if (maze.layout[i][j] == '0') {
                maze.layout[i][j] = 'E';
            }
        }
    }
}

/*  ** ScanLastColumn (Helper Function for FormatMaze
 *
 *  PRE: Maze contains only a 1 or a 0 stored in each coordinate
 *  PST: Any 0s sre replaced with 'E' (for Entry/Exit)
*/

void scanLastColumn (Maze &maze) {
   for (int i = 1; i < maze.layout.size(); ++i) {
       for (int j = maze.layout[i].size() - 1; j < maze.layout[i].size(); ++j) {
           if (maze.layout[i][j] == '0') {
               maze.layout[i][j] = 'E';
           }
       }
   }
}

/*  ** FormatMaze **
 *
 *  PRE: Maze contains only a 1 or a 0 stored in each coordinate
 *  PST: Start and Finish points of the maze are replaced, respectively
 *       with 'S' and 'F'.
*/

void FormatMaze (Maze &maze) {

    scanFirstRow(maze);
    scanLastRow(maze);
    scanFirstColumn(maze);
    scanLastColumn(maze);
    bool StartFound = false;
    for (int i = 0; i < maze.layout.size(); ++i) {
        for (int j = 0; j < maze.layout[i].size(); ++j) {
            if ((StartFound == false) && maze.layout[i][j] == 'E') {
                maze.layout[i][j] = 'S';
                StartFound = true;
            }
            else if ((StartFound == true) && maze.layout[i][j] == 'E') {
                maze.layout[i][j] = 'F';
            }
        }
    }
}
/*  ** PrintMaze**
 *  PRE: Maze contains data in each coordinate
 *  PST: Maze contents are output to screen
*/

void PrintMaze(Maze maze) {
    for (int i = 0; i < maze.layout.size(); ++i) {
        for (int j = 0; j < maze.layout[i].size(); ++j) {
            cout << maze.layout[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

cell FindMazeOpening(Maze &myMaze, char type) {
    cell openingCell;
    for (int i = 0; i < myMaze.layout.size(); ++i) {
        for (int j = 0; j < myMaze.layout[i].size(); ++j) {
            if (myMaze.layout[i][j] == type) {
                openingCell.row = i + OFFSET;
                openingCell.col = j + OFFSET;
            }
        }
    }
    return openingCell;
}

bool ValidPath (Maze &maze, cell queryCell) {
    // Stay within North boundary
    if (queryCell.row < 0) {
        queryCell.row = 0;
        return false;
    }
    // Stay within South boundary
    if (queryCell.row == maze.layout.size()) {
        queryCell.row = maze.layout.size() - 1;
        return false;
    }
    // Stay within East boundary
    if (queryCell.col == maze.layout[0].size()) {
        queryCell.col = maze.layout[0].size() - 1;
        return false;
    }
    // Stay within West boundary
        if (queryCell.col < 0) {
        queryCell.col = 0;
        return false;
    }
    return (maze.layout[queryCell.row][queryCell.col] == '0') ||
           (maze.layout[queryCell.row][queryCell.col] == 'F');
}

cell AdvanceRight (const vector<vector <char>> &Maze, vector<cell> &path, cell queryCell) {
    if (Maze[queryCell.row][queryCell.col] == 'F' ||
        Maze[queryCell.row][queryCell.col] == '0') {
        path.push_back(queryCell);
    }

}

void TraverseMaze(vector<vector<char>> &maze, cell &Start, const cell &Finish) {
    // Use vector to hold traversal path.  Implement as stack.
    vector<cell> crawlerPath;
    cell currentCell = Start;

    //initialize traversal path.
    currentCell.visited = true;
    crawlerPath.push_back(Start);

    while (maze[crawlerPath.back().row][crawlerPath.back().col] != 'F') {

    }

    //check available paths for valid in clockwise order.
    //if path is open, take it.




    // explore according to consistent pattern; if path is blocked, pop
    // current location and resume exploring from previous location.

    // Helper functions, FindStart and FindFinish needed.

    // If traversal coordinates match FindFinish coordinates after completing
    // valid movement, maze is solved.  If no valid exploration paths remain,
    // maze is unsolvable.

}

int main() {

    Maze maze;
    cell startCell;
    cell finishCell;

    maze = ReadMaze("maze.txt");
    FormatMaze(maze);
    startCell = FindMazeOpening(maze, 'S');
    finishCell = FindMazeOpening(maze, 'F');

    PrintMaze(maze);

    cout << "Start is at cell (" << startCell.row << "," << startCell.col << ")" << endl;
    cout << "End is at cell (" << finishCell.row << "," << finishCell.col << ")" << endl;
    return 0;
}