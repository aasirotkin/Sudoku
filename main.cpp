#include "sudoku.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    Sudoku sudoku(
        { 5, 4, 3,  0, 0, 0,  0, 0, 0,
         0, 2, 7,  0, 5, 1,  0, 0, 0,
         0, 0, 8,  0, 7, 0,  2, 5, 6,

         0, 0, 9,  5, 6, 0,  4, 0, 0,
         0, 8, 0,  3, 0, 2,  9, 1, 5,
         0, 0, 2,  1, 0, 7,  6, 8, 0,

         0, 3, 0,  0, 0, 9,  0, 6, 0,
         2, 0, 0,  7, 0, 0,  0, 9, 8,
         0, 0, 4,  0, 8, 5,  0, 3, 0 });

    cout << sudoku;

    return 0;
}
