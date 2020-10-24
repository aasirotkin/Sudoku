#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <tuple>
#include <vector>

class Sudoku
{
public:
    explicit Sudoku(const std::vector<int>& values);

    void NextMove();
    void Solve();

    int& operator() (int row, int col);
    const int& operator() (int row, int col) const;

private:
    Sudoku() = default;

private:
    void CreateSudoku(const std::vector<int>& values);
    void CreatePopularity();
    void CreateSquares();

    void CheckRowInPlace(int row) const;
    void CheckColInPlace(int col) const;
    void CheckRowColInPlace(int row, int col) const;

    void CheckSudokuInPlace() const;
    void CheckRowsInPlace() const;
    void CheckColsInPlace() const;
    void CheckSquaresInPlace() const;

private:
    struct SudokuSquare {
        int row_begin;
        int row_end;
        int col_begin;
        int col_end;

        std::string Str() const;
    };
    std::tuple<bool, std::string> IsRowValid(int row) const;
    std::tuple<bool, std::string> IsColValid(int col) const;
    std::tuple<bool, std::string> IsSquareValid(const SudokuSquare& square) const;

    int MostPopularNumber() const;

private:
    SudokuSquare m_squares[9];
    int m_popularity[9];
    int m_sudoku[9][9];
};

std::ostream& operator<< (std::ostream& out, const Sudoku& square);

#endif // SUDOKU_H
