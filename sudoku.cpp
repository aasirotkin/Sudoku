#include "sudoku.h"

#include <algorithm>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

SudokuError operator+(const SudokuError& lhs, const SudokuError& rhs)
{
    SudokuError error;
    error.is_valid = lhs.is_valid && rhs.is_valid;
    if (!lhs) {
        error.name += lhs.name;
        if (!rhs) {
            error.name += std::string("\n") + rhs.name;
        }
    }
    else {
        if (!rhs) {
            error.name += rhs.name;
        }
    }
    return error;
}

// ----------------------------------------------------------------------------

void SudokuResult::Print() const
{
    for (size_t i = 0; i < solution_steps.size(); ++i) {
        std::cout << i << ") "s << solution_steps.at(i) << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const SudokuResult& result)
{
    if (result.error) {
        std::cout << "Errors:"s << std::endl;
        std::cout << result.error.name << std::endl;
    }
    else {
        std::cout << "Solution steps:"s << std::endl;
        result.Print();
        std::cout << std::endl;
    }
    return out;
}

// ----------------------------------------------------------------------------

std::string SudokuSquare::Str() const
{
    return "{ "s + std::to_string(row_begin) + ", "s + std::to_string(row_end) + ", "s
        + std::to_string(col_begin) + ", "s + std::to_string(col_end) + " }"s;
}

// ----------------------------------------------------------------------------

SudokuGrid::SudokuGrid()
{
    CreateSquares();
}

int& SudokuGrid::operator()(int row, int col) {
    return m_sudoku[Index(row, col)];
}

const int& SudokuGrid::operator()(int row, int col) const {
    return m_sudoku.at(Index(row, col));
}

void SudokuGrid::PushBack(int number)
{
    if (number < 0 || number > 9) {
        throw std::invalid_argument("Invalid number " + std::to_string(number));
    }
    m_sudoku.push_back(number);
}

const std::vector<SudokuSquare>& SudokuGrid::Squares() const
{
    return m_squares;
}

int SudokuGrid::Index(int row, int col) const
{
    IsRowColValid(row, col);
    return row * 9 + col;
}

void SudokuGrid::CreateSquares()
{
    for (int row = 0; row < 9; row = row + 3) {
        for (int col = 0; col < 9; col = col + 3) {
            m_squares.push_back({ row, row + 3, col, col + 3 });
        }
    }
}

void SudokuGrid::IsRowColValid(int row, int col) const
{
    if (row < 0 || row > 8) {
        throw std::invalid_argument("Row " + std::to_string(row) + " must be in [0:9) range"s);
    }
    if (col < 0 || col > 8) {
        throw std::invalid_argument("Col " + std::to_string(col) + " must be in [0:9) range"s);
    }
}

bool operator==(const SudokuGrid& lhs, const SudokuGrid& rhs)
{
    return lhs.Values() == rhs.Values();
}

std::ostream& operator<<(std::ostream& out, const SudokuGrid& grid)
{
    const std::string row_splitter("-------------------------------");
    out << row_splitter << std::endl;
    for (int row = 0; row < 9; ++row) {
        if (row > 0 && row % 3 == 0) {
            out << row_splitter << std::endl;
        }
        out << "|"s;
        for (int col = 0; col < 9; ++col) {
            if (col > 0 && col % 3 == 0) {
                out << "|"s;
            }
            out << " "s << grid(row, col) << " "s;
        }
        out << "|"s << std::endl;
    }
    out << row_splitter << std::endl;
    return out;
}

// ----------------------------------------------------------------------------

Sudoku::Sudoku(const std::vector<int>& values)
{
    CreateSudoku(values);
}

void Sudoku::CreateSudoku(const std::vector<int>& values)
{
    if (values.size() != 81) {
        throw std::invalid_argument("Input vector size must be 81"s);
    }
    for (int number : values) {
        PushBack(number);
    }
}

SudokuError Sudoku::IsSudokuValid() const
{
    SudokuError rows_error = IsRowsValid();
    SudokuError cols_error = IsColsValid();
    SudokuError squares_error = IsSquaresValid();
    return rows_error + cols_error + squares_error;
}

SudokuError Sudoku::IsRowsValid() const
{
    SudokuError error;
    for (int row = 0; row < 9; ++row) {
        error = IsRowValid(row);
        if (!error) {
            break;
        }
    }
    return error;
}

SudokuError Sudoku::IsColsValid() const
{
    SudokuError error;
    for (int col = 0; col < 9; ++col) {
        error = IsColValid(col);
        if (!error) {
            break;
        }
    }
    return error;
}

SudokuError Sudoku::IsSquaresValid() const
{
    SudokuError error;
    for (const SudokuSquare& square : Squares()) {
        error = IsSquareValid(square);
        if (!error) {
            break;
        }
    }
    return error;
}

SudokuError Sudoku::IsRowValid(int row) const
{
    int col_values[9];
    std::fill_n(std::begin(col_values), 9, 0);
    for (int col = 0; col < 9; ++col) {
        int value = (*this)(row, col);
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

SudokuError Sudoku::IsColValid(int col) const
{
    int row_values[9];
    std::fill_n(std::begin(row_values), 9, 0);
    for (int row = 0; row < 9; ++row) {
        int value = (*this)(row, col);
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

SudokuError Sudoku::IsSquareValid(const SudokuSquare& square) const
{
    int square_values[9];
    std::fill_n(std::begin(square_values), 9, 0);
    for (int row = square.row_begin; row < square.row_end; ++row) {
        for (int col = square.col_begin; col < square.col_end; ++col) {
            int value = (*this)(row, col);
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

bool Sudoku::HasRowNumber(int row, int number)
{
    for (int col = 0; col < 9; ++col) {
        if ((*this)(row, col) == number) {
            return true;
        }
    }
    return false;
}

bool Sudoku::HasColNumber(int col, int number)
{
    for (int row = 0; row < 9; ++row) {
        if ((*this)(row, col) == number) {
            return true;
        }
    }
    return false;
}

bool Sudoku::HasSquareNumber(const SudokuSquare& square, int number)
{
    for (int row = square.row_begin; row < square.row_end; ++row) {
        for (int col = square.col_begin; col < square.col_end; ++col) {
            if ((*this)(row, col) == number) {
                return true;
            }
        }
    }
    return false;
}

Sudoku::SudokuFoundPlace Sudoku::SearchForSinglePlaceInSquare(const SudokuSquare& square, int number)
{
    SudokuFoundPlace place = { false, 0, 0 };
    for (int row = square.row_begin; row < square.row_end; ++row) {
        if (HasRowNumber(row, number)) {
            continue;
        }
        for (int col = square.col_begin; col < square.col_end; ++col) {
            if ((*this)(row, col) == 0) {
                if (!HasColNumber(col, number)) {
                    if (!place) {
                        place = { true, row, col };
                    }
                    else {
                        return { false, 0, 0 };
                    }
                }
            }
        }
    }
    return place;
}

// ----------------------------------------------------------------------------

SudokuPopularity::SudokuPopularity(const Sudoku& sudoku)
{
    for (int number = 1; number <= 9; ++number) {
        m_number_popularity.push_back({ number, 0 });
    }
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int number = sudoku(row, col);
            if (number == 0) {
                continue;
            }
            m_number_popularity[number - 1].second += 1;
        }
    }
}

void SudokuPopularity::SortPopularity()
{
    std::sort(m_number_popularity.begin(), m_number_popularity.end(),
        [](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
            if (lhs.second == rhs.second) {
                return lhs.first < rhs.first;
            }
            else {
                return lhs.second > rhs.second;
            } });
}

void SudokuPopularity::ErasePopularity()
{
    m_number_popularity.erase(
        std::remove_if(m_number_popularity.begin(), m_number_popularity.end(),
            [](const std::pair<int, int>& number_popularity) { return number_popularity.second == 9; }),
        m_number_popularity.end());
}

void SudokuPopularity::IncreasePolularity(int number)
{
    for (auto& [value, popularity] : m_number_popularity) {
        if (value == number) {
            popularity++;
            break;
        }
    }
}

// ----------------------------------------------------------------------------

SudokuSolver::SudokuSolver(Sudoku& sudoku)
    : m_sudoku(sudoku)
    , m_popularity(sudoku)
{

}

SudokuResult SudokuSolver::Solve()
{
    SudokuResult result;
    result.error = m_sudoku.IsSudokuValid();
    if (result.error) {
        return result;
    }
    bool res = true;
    while (res == true && !m_popularity.IsEmpty()) {
        m_popularity.SortPopularity();

        res = false;

        for (auto [number, popularity] : m_popularity) {
            if (SolveCrossingOut(number, result.solution_steps)) {
                res = true;
            }
        }

        m_popularity.ErasePopularity();
    }

    if (!res) {
        std::cout << m_sudoku << std::endl;;
    }

    result.error = m_sudoku.IsSudokuValid();
    return result;
}

bool SudokuSolver::SolveCrossingOut(int number, std::vector<std::string>& solutions)
{
    bool res = false;
    for (const auto& square : m_sudoku.Squares()) {
        if (!m_sudoku.HasSquareNumber(square, number)) {
            Sudoku::SudokuFoundPlace place = m_sudoku.SearchForSinglePlaceInSquare(square, number);
            if (place) {
                m_sudoku(place.row, place.col) = number;
                m_popularity.IncreasePolularity(number);
                res = true;
                solutions.push_back("Put number "s + std::to_string(number) + " in row "s + std::to_string(place.row) + " col "s + std::to_string(place.col));
            }
        }
    }
    return res;
}
