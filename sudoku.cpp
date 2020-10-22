#include "sudoku.h"

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
}

int& Sudoku::operator()(int row, int col) {
    auto [global_row, global_col, local_row, local_col] = MapToSquare(row, col);
    return (m_sudoku[global_row][global_col])[local_row][local_col];
}

const int &Sudoku::operator()(int row, int col) const
{
    auto [global_row, global_col, local_row, local_col] = MapToSquare(row, col);
    return (m_sudoku[global_row][global_col])[local_row][local_col];
}

std::tuple<int, int, int, int> Sudoku::MapToSquare(int row, int col) const {
    if (row < 0 || row > 8) {
        throw std::invalid_argument("Row must be in [0:9) range"s);
    }
    if (col < 0 || col > 8) {
        throw std::invalid_argument("Col must be in [0:9) range"s);
    }
    int global_row = row / 3;
    int local_row = row % 3;
    int global_col = col / 3;
    int local_col = col % 3;
    return {global_row, global_col, local_row, local_col};
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
