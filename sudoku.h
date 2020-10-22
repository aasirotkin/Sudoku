#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <tuple>
#include <vector>

using SudokuSquare = int[3][3];

class Sudoku
{
public:
    explicit Sudoku(const std::vector<int>& values);

    int& operator() (int row, int col);
    const int& operator() (int row, int col) const;

private:
    Sudoku() = default;

private:
    std::tuple<int, int, int, int> MapToSquare(int row, int col) const;

private:
    SudokuSquare m_sudoku[3][3];
};

std::ostream& operator<< (std::ostream& out, const Sudoku& square);

#endif // SUDOKU_H
