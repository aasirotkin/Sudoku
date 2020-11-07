#include "sudoku.h"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

SudokuValid operator+(const SudokuValid& lhs, const SudokuValid& rhs)
{
    SudokuValid valid;
    valid.is_valid = lhs.is_valid && rhs.is_valid;
    if (!lhs)
    {
        valid.text += lhs.text;
    }
    if (!rhs)
    {
        if (!lhs) {
            valid.text += "\n"s;
        }
        valid.text += rhs.text;
    }
    return valid;
}

// ----------------------------------------------------------------------------

void SudokuResult::Print() const
{
    for (size_t i = 0; i < solution_steps.size(); ++i)
    {
        std::cout << i << ") "s << solution_steps.at(i) << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const SudokuResult& result)
{
    if (!result.valid)
    {
        std::cout << "Errors:"s << std::endl;
        std::cout << result.valid.text << std::endl;
    }
    else
    {
        std::cout << "Solution steps:"s << std::endl;
        result.Print();
        std::cout << std::endl;
    }
    return out;
}

// ----------------------------------------------------------------------------

std::string SudokuSquare::Str() const
{
    return "{ "s + std::to_string(row_begin) + ", "s + std::to_string(row_end) + ", "s +
        std::to_string(col_begin) + ", "s + std::to_string(col_end) + " }"s;
}

// ----------------------------------------------------------------------------

SudokuGrid::SudokuGrid(const std::vector<int>& values)
{
    FillGrid(values);
    CreateSquares();
}

int& SudokuGrid::operator()(int row, int col)
{
    return m_sudoku[Index(row, col)];
}

const int& SudokuGrid::operator()(int row, int col) const
{
    return m_sudoku.at(Index(row, col));
}

void SudokuGrid::FillGrid(const std::vector<int>& values)
{
    if (values.size() != 81)
    {
        throw std::invalid_argument("Input vector size must be 81"s);
    }
    for (int v : values) {
        if (v < 0 || v > 9) {
            throw std::invalid_argument("Can't fill the grid. Invalid number " + std::to_string(v));
        }
        m_sudoku.push_back(v);
    }
}

std::vector<int> SudokuGrid::Neighbours(int row_col) const
{
    if (row_col < 0 || row_col > 2) {
        throw std::invalid_argument("Can't find a neighbour. Invalid row or col value " + std::to_string(row_col));
    }
    std::vector<int> rows_cols = { 0, 1, 2 };
    rows_cols.erase(std::find(rows_cols.begin(), rows_cols.end(), row_col));
    return rows_cols;
}

int SudokuGrid::Index(int row, int col) const
{
    IsRowColValid(row, col);
    return row * 9 + col;
}

void SudokuGrid::CreateSquares()
{
    for (int row = 0, local_row = 0; row < 9; row = row + 3, ++local_row)
    {
        for (int col = 0, local_col = 0; col < 9; col = col + 3, ++local_col)
        {
            m_squares.push_back({ row, row + 3, col, col + 3, local_row, local_col });
        }
    }
}

void SudokuGrid::IsRowColValid(int row, int col) const
{
    if (row < 0 || row > 8)
    {
        throw std::invalid_argument("Row " + std::to_string(row) + " must be in [0:9) range"s);
    }
    if (col < 0 || col > 8)
    {
        throw std::invalid_argument("Col " + std::to_string(col) + " must be in [0:9) range"s);
    }
}

bool operator==(const SudokuGrid& lhs, const SudokuGrid& rhs)
{
    return lhs.Values() == rhs.Values();
}

bool operator!=(const SudokuGrid& lhs, const SudokuGrid& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& out, const SudokuGrid& grid)
{
    const std::string row_splitter("-------------------------------");
    out << row_splitter << std::endl;
    for (int row = 0; row < 9; ++row)
    {
        if (row > 0 && row % 3 == 0)
        {
            out << row_splitter << std::endl;
        }
        out << "|"s;
        for (int col = 0; col < 9; ++col)
        {
            if (col > 0 && col % 3 == 0)
            {
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

SudokuValid SudokuCheckValidity::IsSudokuValid(const SudokuGrid& grid)
{
    SudokuValid rows_valid = IsRowsValid(grid);
    SudokuValid cols_valid = IsColsValid(grid);
    SudokuValid squares_valid = IsSquaresValid(grid);
    return rows_valid + cols_valid + squares_valid;
}

SudokuValid SudokuCheckValidity::IsRowsValid(const SudokuGrid& grid)
{
    SudokuValid valid;
    for (int row = 0; row < 9; ++row)
    {
        valid = IsRowValid(grid, row);
        if (!valid)
        {
            break;
        }
    }
    return valid;
}

SudokuValid SudokuCheckValidity::IsColsValid(const SudokuGrid& grid)
{
    SudokuValid valid;
    for (int col = 0; col < 9; ++col)
    {
        valid = IsColValid(grid, col);
        if (!valid)
        {
            break;
        }
    }
    return valid;
}

SudokuValid SudokuCheckValidity::IsSquaresValid(const SudokuGrid& grid)
{
    SudokuValid valid;
    for (const SudokuSquare& square : grid.Squares())
    {
        valid = IsSquareValid(grid, square);
        if (!valid)
        {
            break;
        }
    }
    return valid;
}

SudokuValid SudokuCheckValidity::IsRowValid(const SudokuGrid& grid, int row)
{
    int col_values[9];
    std::fill_n(std::begin(col_values), 9, 0);
    for (int col = 0; col < 9; ++col)
    {
        int value = grid(row, col);
        if (value == 0)
        {
            continue;
        }
        int index = value - 1;
        if (col_values[index] > 0)
        {
            return { false, "Number "s + std::to_string(value) + " has appeared in the row "s + std::to_string(row) +
                            " at least twice"s };
        }
        col_values[index] = value;
    }
    return { true, ""s };
}

SudokuValid SudokuCheckValidity::IsColValid(const SudokuGrid& grid, int col)
{
    int row_values[9];
    std::fill_n(std::begin(row_values), 9, 0);
    for (int row = 0; row < 9; ++row)
    {
        int value = grid(row, col);
        if (value == 0)
        {
            continue;
        }
        int index = value - 1;
        if (row_values[index] > 0)
        {
            return { false, "Number "s + std::to_string(value) + " has appeared in the col "s + std::to_string(col) +
                            " at least twice"s };
        }
        row_values[index] = value;
    }
    return { true, ""s };
}

SudokuValid SudokuCheckValidity::IsSquareValid(const SudokuGrid& grid, const SudokuSquare& square)
{
    int square_values[9];
    std::fill_n(std::begin(square_values), 9, 0);
    for (int row = square.row_begin; row < square.row_end; ++row)
    {
        for (int col = square.col_begin; col < square.col_end; ++col)
        {
            int value = grid(row, col);
            if (value == 0)
            {
                continue;
            }
            int index = value - 1;
            if (square_values[index] > 0)
            {
                return { false, "Number "s + std::to_string(value) + " has appeared in the square "s + square.Str() +
                                " at least twice"s };
            }
            square_values[index] = value;
        }
    }
    return { true, ""s };
}

// ----------------------------------------------------------------------------

Sudoku::Sudoku(const std::vector<int>& values)
    : SudokuGrid(values)
{

}

SudokuValid Sudoku::IsSudokuValid() const
{
    return SudokuCheckValidity::IsSudokuValid(*this);
}

bool Sudoku::HasRowNumber(int row, int number) const
{
    for (int col = 0; col < 9; ++col)
    {
        if ((*this)(row, col) == number)
        {
            return true;
        }
    }
    return false;
}

bool Sudoku::HasColNumber(int col, int number) const
{
    for (int row = 0; row < 9; ++row)
    {
        if ((*this)(row, col) == number)
        {
            return true;
        }
    }
    return false;
}

bool Sudoku::HasSquareNumber(const SudokuSquare& square, int number) const
{
    for (int row = square.row_begin; row < square.row_end; ++row)
    {
        for (int col = square.col_begin; col < square.col_end; ++col)
        {
            if ((*this)(row, col) == number)
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<int> Sudoku::AvailableRows(int number, const SudokuSquare& square) const
{
    std::vector<int> available_rows;
    if (HasSquareNumber(square, number)) {
        return available_rows;
    }
    for (int row = square.row_begin; row < square.row_end; ++row)
    {
        if (!HasRowNumber(row, number))
        {
            bool all_places_are_busy = true;
            for (int col = square.col_begin; col < square.col_end; ++col)
            {
                if ((*this)(row, col) == 0 && !HasColNumber(col, number))
                {
                    all_places_are_busy = false;
                    break;
                }
            }
            if (!all_places_are_busy)
            {
                available_rows.push_back(row);
            }
        }
    }
    return available_rows;
}

std::vector<int> Sudoku::AvailableCols(int number, const SudokuSquare& square) const
{
    std::vector<int> available_cols;
    if (HasSquareNumber(square, number)) {
        return available_cols;
    }
    for (int col = square.col_begin; col < square.col_end; ++col)
    {
        if (!HasColNumber(col, number))
        {
            bool all_places_are_busy = true;
            for (int row = square.row_begin; row < square.row_end; ++row)
            {
                if ((*this)(row, col) == 0 && !HasRowNumber(row, number))
                {
                    all_places_are_busy = false;
                    break;
                }
            }
            if (!all_places_are_busy)
            {
                available_cols.push_back(col);
            }
        }
    }
    return available_cols;
}

Sudoku::SudokuFoundPlace Sudoku::SearchUsingCrossingOut(const SudokuSquare& square, int number)
{
    SudokuFoundPlace place = { false, 0, 0 };
    for (int row = square.row_begin; row < square.row_end; ++row)
    {
        if (HasRowNumber(row, number))
        {
            continue;
        }
        for (int col = square.col_begin; col < square.col_end; ++col)
        {
            if ((*this)(row, col) == 0)
            {
                if (!HasColNumber(col, number))
                {
                    if (!place)
                    {
                        place = { true, row, col };
                    }
                    else
                    {
                        return { false, 0, 0 };
                    }
                }
            }
        }
    }
    return place;
}

Sudoku::SudokuFoundPlace Sudoku::SearchUsingDoubleGuess(const SudokuSquare& square, int number)
{
    std::vector<int> res_v;
    const std::vector<int> col_neighbours = Neighbours(square.col);
    const int start_index = square.row * 3;
    const int first_neighbour_index = start_index + col_neighbours.at(0);
    const int second_neighbour_index = start_index + col_neighbours.at(1);

    std::vector<int> available_rows_0 = AvailableRows(number, square);
    const std::vector<int> available_rows_1 = AvailableRows(number, Squares().at(first_neighbour_index));
    const std::vector<int> available_rows_2 = AvailableRows(number, Squares().at(second_neighbour_index));

    if (available_rows_1.size() == 2 && available_rows_1 == available_rows_2 && available_rows_0 != available_rows_1)
    {
        std::set_difference(available_rows_0.begin(), available_rows_0.end(), available_rows_1.begin(),
            available_rows_1.end(), std::back_inserter(res_v));
    }
    else
    {
        if (available_rows_0.size() == 2)
        {
            if (available_rows_1.size() == 1)
            {
                std::set_difference(available_rows_0.begin(), available_rows_0.end(), available_rows_1.begin(),
                    available_rows_1.end(), std::back_inserter(res_v));
            }
        }
        if (available_rows_0.size() == 2)
        {
            if (available_rows_2.size() == 1)
            {
                std::set_difference(available_rows_0.begin(), available_rows_0.end(), available_rows_2.begin(),
                    available_rows_2.end(), std::back_inserter(res_v));
            }
        }
    }

    if (res_v.size() != 1)
    {
        return { false, 0, 0 };
    }

    int final_row = res_v.front();
    int final_col = 0;
    bool res = false;
    for (int col = square.col_begin; col < square.col_end; ++col)
    {
        if ((*this)(final_row, col) == 0 && !HasColNumber(col, number))
        {
            if (res)
            {
                return { false, 0, 0 };
            }
            final_col = col;
            res = true;
        }
    }
    return { res, final_row, final_col };
}

Sudoku::SudokuFoundPlace Sudoku::SearchUsingTripleGuess(const SudokuSquare& square, int number)
{
    std::vector<int> res_v;
    const std::vector<int> row_neighbours = Neighbours(square.row);
    const int start_index = square.col;
    const int first_neighbour_index = start_index + row_neighbours.at(0) * 3;
    const int second_neighbour_index = start_index + row_neighbours.at(1) * 3;

    std::vector<int> available_cols_0 = AvailableCols(number, square);
    const std::vector<int> available_cols_1 = AvailableCols(number, Squares().at(first_neighbour_index));
    const std::vector<int> available_cols_2 = AvailableCols(number, Squares().at(second_neighbour_index));

    if (available_cols_1.size() == 2 && available_cols_1 == available_cols_2 && available_cols_0 != available_cols_1)
    {
        std::set_difference(available_cols_0.begin(), available_cols_0.end(), available_cols_1.begin(),
            available_cols_1.end(), std::back_inserter(res_v));
    }
    else
    {
        if (available_cols_0.size() == 2)
        {
            if (available_cols_1.size() == 1)
            {
                std::set_difference(available_cols_0.begin(), available_cols_0.end(), available_cols_1.begin(),
                    available_cols_1.end(), std::back_inserter(res_v));
            }
        }
        if (available_cols_0.size() == 2)
        {
            if (available_cols_2.size() == 1)
            {
                std::set_difference(available_cols_0.begin(), available_cols_0.end(), available_cols_2.begin(),
                    available_cols_2.end(), std::back_inserter(res_v));
            }
        }
    }

    if (res_v.size() != 1)
    {
        return { false, 0, 0 };
    }

    int final_row = 0;
    int final_col = res_v.front();
    bool res = false;
    for (int row = square.row_begin; row < square.row_end; ++row)
    {
        if ((*this)(row, final_col) == 0 && !HasRowNumber(row, number))
        {
            if (res)
            {
                return { false, 0, 0 };
            }
            final_row = row;
            res = true;
        }
    }
    return { res, final_row, final_col };
}

// ----------------------------------------------------------------------------

SudokuPopularity::SudokuPopularity(const Sudoku& sudoku)
{
    for (int number = 1; number <= 9; ++number)
    {
        m_number_popularity.push_back({ number, 0 });
    }
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            int number = sudoku(row, col);
            if (number == 0)
            {
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
            if (lhs.second == rhs.second)
            {
                return lhs.first < rhs.first;
            }
            else
            {
                return lhs.second > rhs.second;
            }
        });
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
    for (auto& [value, popularity] : m_number_popularity)
    {
        if (value == number)
        {
            popularity++;
            break;
        }
    }
}

// ----------------------------------------------------------------------------

SudokuSolver::SudokuSolver(Sudoku& sudoku) : m_sudoku(sudoku), m_popularity(sudoku)
{
}

SudokuResult SudokuSolver::Solve()
{
    int count = 0;
    SudokuResult result;
    result.valid = m_sudoku.IsSudokuValid();
    if (!result.valid)
    {
        return result;
    }
    bool res = true;
    while (res == true && !m_popularity.IsEmpty())
    {
        m_popularity.SortPopularity();

        res = false;

        if (!res)
        {
            for (auto [number, popularity] : m_popularity)
            {
                if (SolveCrossingOut(number, result.solution_steps))
                {
                    res = true;
                }
            }
        }

        if (!res)
        {
            for (auto [number, popularity] : m_popularity)
            {
                if (SolveDoubleGuess(number, result.solution_steps))
                {
                    res = true;
                }
            }
        }

        if (!res)
        {
            for (auto [number, popularity] : m_popularity)
            {
                if (SolveTripleGuess(number, result.solution_steps))
                {
                    res = true;
                }
            }
        }

        if (++count == 25) {
            throw std::runtime_error("Count has achived it's maximum value that equals 25");
        }

        m_popularity.ErasePopularity();
    }

    result.valid = m_sudoku.IsSudokuValid();
    return result;
}

bool SudokuSolver::SolveCrossingOut(int number, std::vector<std::string>& solutions)
{
    bool res = false;
    for (const auto& square : m_sudoku.Squares())
    {
        if (!m_sudoku.HasSquareNumber(square, number))
        {
            Sudoku::SudokuFoundPlace place = m_sudoku.SearchUsingCrossingOut(square, number);
            if (place)
            {
                m_sudoku(place.row, place.col) = number;
                m_popularity.IncreasePolularity(number);
                res = true;
                solutions.push_back("Put number "s + std::to_string(number) + " in row "s + std::to_string(place.row) +
                    " col "s + std::to_string(place.col));
            }
        }
    }
    return res;
}

bool SudokuSolver::SolveDoubleGuess(int number, std::vector<std::string>& solutions)
{
    bool res = false;
    for (const auto& square : m_sudoku.Squares())
    {
        if (!m_sudoku.HasSquareNumber(square, number))
        {
            Sudoku::SudokuFoundPlace place = m_sudoku.SearchUsingDoubleGuess(square, number);
            if (place)
            {
                m_sudoku(place.row, place.col) = number;
                m_popularity.IncreasePolularity(number);
                res = true;
                solutions.push_back("Put number "s + std::to_string(number) + " in row "s + std::to_string(place.row) +
                    " col "s + std::to_string(place.col));
            }
        }
    }
    return res;
}

bool SudokuSolver::SolveTripleGuess(int number, std::vector<std::string>& solutions)
{
    bool res = false;
    for (const auto& square : m_sudoku.Squares())
    {
        if (!m_sudoku.HasSquareNumber(square, number))
        {
            Sudoku::SudokuFoundPlace place = m_sudoku.SearchUsingTripleGuess(square, number);
            if (place)
            {
                m_sudoku(place.row, place.col) = number;
                m_popularity.IncreasePolularity(number);
                res = true;
                solutions.push_back("Put number "s + std::to_string(number) + " in row "s + std::to_string(place.row) +
                    " col "s + std::to_string(place.col));
            }
        }
    }
    return res;
}
