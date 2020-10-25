#include "sudoku.h"

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

int main()
{
    vector<int> sudoku_test_data = {
        5, 4, 3,  0, 0, 0,  0, 0, 0,
        0, 2, 7,  0, 5, 1,  0, 0, 0,
        0, 0, 8,  0, 7, 0,  2, 5, 6,

        0, 0, 9,  5, 6, 0,  4, 0, 0,
        0, 8, 0,  3, 0, 2,  9, 1, 5,
        0, 0, 2,  1, 0, 7,  6, 8, 0,

        0, 3, 0,  0, 0, 9,  0, 6, 0,
        2, 0, 0,  7, 0, 0,  0, 9, 8,
        0, 0, 4,  0, 8, 5,  0, 3, 0 };

    try {
        Sudoku sudoku(sudoku_test_data);
        cout << "Input:"s << endl;
        cout << sudoku << endl;
        SudokuSolver solver(sudoku);
        SudokuResult result = solver.Solve();
        cout << "Solution:"s << endl;
        result.Print();
        cout << "Result:"s << endl;
        cout << sudoku << endl;
        if (!result) {
            cout << endl << "Errors:"s << endl;
            cout << result.error << endl;
        }
    }
    catch (const invalid_argument& error) {
        cout << "Error: "s << error.what() << endl;
    }
    catch (...) {
        cout << "Unknown error"s << endl;
    }

    return 0;
}
