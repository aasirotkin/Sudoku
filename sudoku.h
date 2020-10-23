#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <tuple>
#include <vector>

class Sudoku
{
public:
    explicit Sudoku(const std::vector<int>& values);

    int& operator() (int row, int col);
    const int& operator() (int row, int col) const;

private:
    Sudoku() = default;

private:
    void CheckRowColInPlace(int row, int col) const;
    void CheckSudokuInPlace() const;
    void CheckRowsInPlace() const;
    void CheckColsInPlace() const;
    void CheckSquareInPlace(int row_begin, int row_end, int col_begin, int col_end, const std::string& square_name) const;

private:
    int m_sudoku[9][9];
};

std::ostream& operator<< (std::ostream& out, const Sudoku& square);

#endif // SUDOKU_H
