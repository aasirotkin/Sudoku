#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <tuple>
#include <vector>

struct SudokuValid
{
    bool is_valid = false;
    std::string text = std::string("");

    operator bool() const
    {
        return is_valid;
    }
};

SudokuValid operator+(const SudokuValid& lhs, const SudokuValid& rhs);

// ----------------------------------------------------------------------------

struct SudokuResult
{
    SudokuValid valid = SudokuValid();
    std::vector<std::string> solution_steps;

    SudokuResult() = default;

    void Print() const;

    operator bool() const
    {
        return valid;
    }
};

std::ostream& operator<<(std::ostream& out, const SudokuResult& result);

// ----------------------------------------------------------------------------

struct SudokuSquare
{
    // global (in grid 9x9)
    int row_begin;
    int row_end;
    int col_begin;
    int col_end;

    // local (in grid 3x3)
    int row;
    int col;

    std::string Str() const;
};

// ----------------------------------------------------------------------------

class SudokuGrid
{
public:
    SudokuGrid(const std::vector<int>& values);

    int& operator()(int row, int col);
    const int& operator()(int row, int col) const;

    void FillGrid(const std::vector<int>& values);

    const std::vector<SudokuSquare>& Squares() const {
        return m_squares;
    }

    const std::vector<int>& Values() const {
        return m_sudoku;
    }

    std::vector<int> Neighbours(int row_col) const;

private:
    int Index(int row, int col) const;
    void CreateSquares();
    void IsRowColValid(int row, int col) const;

private:
    std::vector<SudokuSquare> m_squares;
    std::vector<int> m_sudoku;
};

bool operator==(const SudokuGrid& lhs, const SudokuGrid& rhs);
bool operator!=(const SudokuGrid& lhs, const SudokuGrid& rhs);

std::ostream& operator<<(std::ostream& out, const SudokuGrid& grid);

// ----------------------------------------------------------------------------

class SudokuCheckValidity
{
public:
    static SudokuValid IsSudokuValid(const SudokuGrid& grid);

private:
    static SudokuValid IsRowsValid(const SudokuGrid& grid);
    static SudokuValid IsColsValid(const SudokuGrid& grid);
    static SudokuValid IsSquaresValid(const SudokuGrid& grid);

    static SudokuValid IsRowValid(const SudokuGrid& grid, int row);
    static SudokuValid IsColValid(const SudokuGrid& grid, int col);
    static SudokuValid IsSquareValid(const SudokuGrid& grid, const SudokuSquare& square);
};

// ----------------------------------------------------------------------------

class Sudoku : public SudokuGrid
{
public:
    explicit Sudoku(const std::vector<int>& values);

    SudokuValid IsSudokuValid() const;

    bool HasRowNumber(int row, int number) const;
    bool HasColNumber(int col, int number) const;
    bool HasSquareNumber(const SudokuSquare& square, int number) const;

    std::vector<int> AvailableRows(int number, const SudokuSquare& square) const;

    struct SudokuFoundPlace
    {
        bool was_found;
        int row;
        int col;

        operator bool() const
        {
            return was_found;
        }
    };

    Sudoku::SudokuFoundPlace SearchUsingCrossingOut(const SudokuSquare& square, int number);
    Sudoku::SudokuFoundPlace SearchUsingDoubleGuess(const SudokuSquare& square, int number);

private:
    Sudoku() = default;
};

// ----------------------------------------------------------------------------

class SudokuPopularity
{
public:
    SudokuPopularity(const Sudoku& sudoku);

    void SortPopularity();
    void ErasePopularity();
    void IncreasePolularity(int number);

    bool IsEmpty() const
    {
        return m_number_popularity.empty();
    }

    const auto begin() const
    {
        return m_number_popularity.begin();
    }

    const auto end() const
    {
        return m_number_popularity.end();
    }

private:
    std::vector<std::pair<int, int>> m_number_popularity;
};

// ----------------------------------------------------------------------------

class SudokuSolver
{
public:
    SudokuSolver(Sudoku& sudoku);

    SudokuResult Solve();

private:
    bool SolveCrossingOut(int number, std::vector<std::string>& solutions);
    bool SolveDoubleGuess(int number, std::vector<std::string>& solutions);

private:
    Sudoku& m_sudoku;
    SudokuPopularity m_popularity;
};

#endif // SUDOKU_H
