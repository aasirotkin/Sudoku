#include "sudoku.h"
#include "sudoku_test.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    SudokuTest::TestSudoku();

    vector<int> example = {
        3, 0, 5,  6, 7, 0,  0, 0, 8,
        0, 0, 8,  0, 9, 0,  2, 0, 1,
        0, 0, 9,  5, 0, 0,  0, 0, 6,

        0, 0, 0,  0, 0, 0,  0, 0, 0,
        0, 0, 0,  4, 5, 0,  0, 9, 7,
        0, 0, 0,  2, 6, 0,  5, 0, 0,

        0, 0, 0,  0, 0, 0,  3, 1, 0,
        0, 8, 0,  0, 0, 6,  0, 0, 0,
        0, 4, 0,  0, 0, 1,  0, 0, 0
    };

    Sudoku sudoku(example);
    SudokuSolver solver(sudoku);
    SudokuResult res = solver.Solve();

    cout << endl << res << endl;

    return 0;
}
