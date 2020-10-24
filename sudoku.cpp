#include "sudoku.h"

#include <algorithm>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

Sudoku::Sudoku(const std::vector<int>& values)
{
    CreateSudoku(values);
    CreatePopularity();
    CreateSquares();
    CheckSudokuInPlace();
}

void Sudoku::NextMove()
{
    int popular_number = MostPopularNumber();
    std::cout << popular_number << std::endl;
}

void Sudoku::Solve()
{
    NextMove();
}

int& Sudoku::operator()(int row, int col) {
    CheckRowColInPlace(row, col);
    return m_sudoku[row][col];
}

const int& Sudoku::operator()(int row, int col) const {
    CheckRowColInPlace(row, col);
    return m_sudoku[row][col];
}

void Sudoku::CreateSudoku(const std::vector<int>& values)
{
    if (values.size() != 81) {
        throw std::invalid_argument("Input vector size must be 81"s);
    }
    for (int i = 0; i < 81; ++i) {
        int value = values.at(i);
        int row = i / 9;
        int col = i - row * 9;
        if (value < 0 || value > 9) {
            throw std::invalid_argument("Invalid number " + std::to_string(value) + " in row "s + std::to_string(row) + " col "s + std::to_string(col));
        }
        this->operator()(row, col) = value;
    }
}

void Sudoku::CreateSquares()
{
    int square_index = 0;
    for (int row = 0; row < 9; row = row + 3) {
        for (int col = 0; col < 9; col = col + 3) {
            m_squares[square_index] = { row, row + 3, col, col + 3 };
            ++square_index;
        }
    }
}

void Sudoku::CreatePopularity()
{
    std::fill_n(std::begin(m_popularity), 9, 0);
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int value = m_sudoku[row][col];
            if (value == 0) {
                continue;
            }
            int index = value - 1;
            m_popularity[index]++;
        }
    }
}

void Sudoku::CheckRowInPlace(int row) const
{
    if (row < 0 || row > 8) {
        throw std::invalid_argument("Row " + std::to_string(row) + " must be in [0:9) range"s);
    }
}

void Sudoku::CheckColInPlace(int col) const
{
    if (col < 0 || col > 8) {
        throw std::invalid_argument("Col " + std::to_string(col) + " must be in [0:9) range"s);
    }
}

void Sudoku::CheckRowColInPlace(int row, int col) const
{
    CheckRowInPlace(row);
    CheckColInPlace(col);
}

void Sudoku::CheckSudokuInPlace() const
{
    CheckRowsInPlace();
    CheckColsInPlace();
    CheckSquaresInPlace();
}

void Sudoku::CheckRowsInPlace() const
{
    for (int row = 0; row < 9; ++row) {
        auto [isValid, error] = IsRowValid(row);
        if (!isValid) {
            throw std::invalid_argument(error);
        }
    }
}

void Sudoku::CheckColsInPlace() const
{
    for (int col = 0; col < 9; ++col) {
        auto [isValid, error] = IsColValid(col);
        if (!isValid) {
            throw std::invalid_argument(error);
        }
    }
}

void Sudoku::CheckSquaresInPlace() const
{
    for (auto i = std::begin(m_squares); i != std::end(m_squares); ++i) {
        auto [isValid, error] = IsSquareValid(*i);
        if (!isValid) {
            throw std::invalid_argument(error);
        }
    }
}

std::string Sudoku::SudokuSquare::Str() const
{
    return "{ "s + std::to_string(row_begin) + ", "s + std::to_string(row_end) + ", "s
        + std::to_string(col_begin) + ", "s + std::to_string(col_end) + " }"s;
}

std::tuple<bool, std::string> Sudoku::IsRowValid(int row) const
{
    CheckRowInPlace(row);
    int col_values[9];
    std::fill_n(std::begin(col_values), 9, 0);
    for (int col = 0; col < 9; ++col) {
        int value = m_sudoku[row][col];
        if (value == 0) {
            continue;
        }
        int index = value - 1;
        if (col_values[index] > 0) {
            return { false,
                "Number "s + std::to_string(value) +
                " has appeared in the row "s + std::to_string(row) + " at least twice"s };
        }
        col_values[index] = value;
    }
    return { true, ""s };
}

std::tuple<bool, std::string> Sudoku::IsColValid(int col) const
{
    CheckColInPlace(col);
    int row_values[9];
    std::fill_n(std::begin(row_values), 9, 0);
    for (int row = 0; row < 9; ++row) {
        int value = m_sudoku[row][col];
        if (value == 0) {
            continue;
        }
        int index = value - 1;
        if (row_values[index] > 0) {
            return { false,
                "Number "s + std::to_string(value) +
                " has appeared in the col "s + std::to_string(col) + " at least twice"s };
        }
        row_values[index] = value;
    }
    return { true, ""s };
}

std::tuple<bool, std::string> Sudoku::IsSquareValid(const SudokuSquare& square) const
{
    int square_values[9];
    std::fill_n(std::begin(square_values), 9, 0);
    for (int row = square.row_begin; row < square.row_end; ++row) {
        for (int col = square.col_begin; col < square.col_end; ++col) {
            int value = m_sudoku[row][col];
            if (value == 0) {
                continue;
            }
            int index = value - 1;
            if (square_values[index] > 0) {
                return { false,
                    "Number "s + std::to_string(value) +
                    " has appeared in the square "s + square.Str() + " at least twice"s };
            }
            square_values[index] = value;
        }
    }
    return { true, ""s };
}

int Sudoku::MostPopularNumber() const {
    return (std::max_element(std::begin(m_popularity), std::end(m_popularity)) - std::begin(m_popularity)) + 1;
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
