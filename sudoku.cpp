#include "sudoku.h"

#include <algorithm>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

Sudoku::Sudoku(const std::vector<int>& values)
{
    if (values.size() != 81) {
        throw std::invalid_argument("Input vector size must be 81"s);
    }
    for (int i = 0; i < 81; ++i) {
        int row = i / 9;
        int col = i - row * 9;
        this->operator()(row, col) = values.at(i);
    }
    CheckSudokuInPlace();
}

int& Sudoku::operator()(int row, int col) {
    CheckRowColInPlace(row, col);
    return m_sudoku[row][col];
}

const int& Sudoku::operator()(int row, int col) const {
    CheckRowColInPlace(row, col);
    return m_sudoku[row][col];
}

void Sudoku::CheckRowColInPlace(int row, int col) const
{
    if (row < 0 || row > 8) {
        throw std::invalid_argument("Row must be in [0:9) range"s);
    }
    if (col < 0 || col > 8) {
        throw std::invalid_argument("Col must be in [0:9) range"s);
    }
}

void Sudoku::CheckSudokuInPlace() const
{
    CheckRowsInPlace();
    CheckColsInPlace();
    CheckSquareInPlace(0, 3, 0, 3, "1"s);
    CheckSquareInPlace(0, 3, 3, 6, "2"s);
    CheckSquareInPlace(0, 3, 6, 9, "3"s);
    CheckSquareInPlace(3, 6, 0, 3, "4"s);
    CheckSquareInPlace(3, 6, 3, 6, "5"s);
    CheckSquareInPlace(3, 6, 6, 9, "6"s);
    CheckSquareInPlace(6, 9, 0, 3, "7"s);
    CheckSquareInPlace(6, 9, 3, 6, "8"s);
    CheckSquareInPlace(6, 9, 6, 9, "9"s);
}

void Sudoku::CheckRowsInPlace() const
{
    int col_values[9];
    for (int row = 0; row < 9; ++row) {
        std::fill_n(std::begin(col_values), 9, 0);
        for (int col = 0; col < 9; ++col) {
            int value = m_sudoku[row][col];
            int index = (value > 0) ? (value - 1) : value;
            if (index > 0 && col_values[index] > 0) {
                throw std::invalid_argument(
                    "Number "s + std::to_string(value) +
                    " has appeared in the row "s + std::to_string(row) + " at least twice"s);
            }
            col_values[index] = value;
        }
    }
}

void Sudoku::CheckColsInPlace() const
{
    int row_values[9];
    for (int col = 0; col < 9; ++col) {
        std::fill_n(std::begin(row_values), 9, 0);
        for (int row = 0; row < 9; ++row) {
            int value = m_sudoku[row][col];
            int index = (value > 0) ? (value - 1) : value;
            if (index > 0 && row_values[index] > 0) {
                throw std::invalid_argument(
                    "Number "s + std::to_string(value) +
                    " has appeared in the col "s + std::to_string(col) + " at least twice"s);
            }
            row_values[index] = value;
        }
    }
}

void Sudoku::CheckSquareInPlace(int row_begin, int row_end, int col_begin, int col_end, const std::string& square_name) const
{
    if ((row_end - row_begin) != 3) {
        throw std::invalid_argument("The number of rows is expected to be 3"s);
    }
    if ((col_end - col_begin) != 3) {
        throw std::invalid_argument("The number of cols is expected to be 3"s);
    }
    int square_values[9];
    std::fill_n(std::begin(square_values), 9, 0);
    for (int row = row_begin; row < row_end; ++row) {
        for (int col = col_begin; col < col_end; ++col) {
            int value = m_sudoku[row][col];
            int index = (value > 0) ? (value - 1) : value;
            if (index > 0 && square_values[index] > 0) {
                throw std::invalid_argument(
                    "Number "s + std::to_string(value) +
                    " has appeared in the square "s + square_name + " at least twice"s);
            }
            square_values[index] = value;
        }
    }
}

std::ostream& operator<< (std::ostream& out, const Sudoku& sudoku) {
    const std::string row_splitter("-------------------------------");
    out << row_splitter << std::endl;
    for (int i = 0; i < 9; ++i) {
        if (i > 0 && i % 3 == 0) {
            out << row_splitter << std::endl;
        }
        out << "|"s;
        for (int j = 0; j < 9; ++j) {
            if (j > 0 && j % 3 == 0) {
                out << "|"s;
            }
            out << " "s << sudoku(i, j) << " "s;
        }
        out << "|"s << std::endl;
    }
    out << row_splitter << std::endl;
    return out;
}
