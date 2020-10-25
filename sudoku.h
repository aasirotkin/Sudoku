#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <tuple>
#include <vector>

struct SudokuError
{
    bool is_valid = true;
    std::string name = std::string("");

    operator bool() const {
        return is_valid;
    }
};

SudokuError operator+ (const SudokuError& lhs, const SudokuError& rhs);

// ----------------------------------------------------------------------------

struct SudokuResult
{
    SudokuError error = SudokuError();
    std::vector<std::string> solution_steps;

    SudokuResult() = default;

    void Print() const;

    operator bool() const {
        return error;
    }
};

// ----------------------------------------------------------------------------

struct SudokuSquare {
    int row_begin;
    int row_end;
    int col_begin;
    int col_end;

    std::string Str() const;
};

// ----------------------------------------------------------------------------

class SudokuGrid
{
public:
    SudokuGrid();

    int& operator() (int row, int col);
    const int& operator() (int row, int col) const;

    void PushBack(int number);

    const std::vector<SudokuSquare>& Squares() const;

    const std::vector<int>& Values() const {
        return m_sudoku;
    }

private:
    int Index(int row, int col) const;
    void CreateSquares();
    void IsRowColValid(int row, int col) const;

private:
    std::vector<SudokuSquare> m_squares;
    std::vector<int> m_sudoku;
};

bool operator== (const SudokuGrid& lhs, const SudokuGrid& rhs);

// ----------------------------------------------------------------------------

class Sudoku
{
public:
    explicit Sudoku(const std::vector<int>& values);

    int operator() (int row, int col) const {
        return m_grid(row, col);
    }

    int& operator() (int row, int col) {
        return m_grid(row, col);
    }

    SudokuError IsSudokuValid() const;

    SudokuError IsRowsValid() const;
    SudokuError IsColsValid() const;
    SudokuError IsSquaresValid() const;

    SudokuError IsRowValid(int row) const;
    SudokuError IsColValid(int col) const;
    SudokuError IsSquareValid(const SudokuSquare& square) const;

    bool HasRowNumber(int row, int number);
    bool HasColNumber(int col, int number);
    bool HasSquareNumber(const SudokuSquare& square, int number);

    struct SudokuFoundPlace {
        bool has_found;
        int row;
        int col;

        operator bool() const {
            return has_found;
        }
    };

    SudokuFoundPlace SearchForSinglePlaceInSquare(const SudokuSquare& square, int number);

    const std::vector<SudokuSquare>& Squares() const {
        return m_grid.Squares();
    }

    const SudokuGrid& Grid() const {
        return m_grid;
    }

private:
    Sudoku() = default;

private:
    void CreateSudoku(const std::vector<int>& values);

private:
    SudokuGrid m_grid;
};

std::ostream& operator<< (std::ostream& out, const Sudoku& sudoku);

bool operator== (const Sudoku& lhs, const Sudoku& rhs);

// ----------------------------------------------------------------------------

class SudokuSolver
{
public:
    SudokuSolver(Sudoku& sudoku);

    SudokuResult Solve();

private:
    void CreatePopularity(Sudoku& sudoku);
    void IncreasePolularity(int number);

    bool SolveCrossingOut(int number, std::vector<std::string>& solutions);

private:
    Sudoku& m_sudoku;
    std::vector<std::pair<int, int>> m_number_popularity;
};


#endif // SUDOKU_H
