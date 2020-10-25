#include "sudoku_test.h"

#include "sudoku.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std::string_literals;

// http://www.sudoku-download.net/files/60_Sudokus_Pattern_Easy.pdf
// http://www.sudoku-download.net/files/Solution_60_Sudokus_Pattern_Easy.pdf

static SudokuTest::SudokuTestData data_easy = {
    { // 1
        {
            7,2,3, 0,0,0, 1,5,9,
            6,0,0, 3,0,2, 0,0,8,
            8,0,0, 0,1,0, 0,0,2,

            0,7,0, 6,5,4, 0,2,0,
            0,0,4, 2,0,7, 3,0,0,
            0,5,0, 9,3,1, 0,4,0,

            5,0,0, 0,7,0, 0,0,3,
            4,0,0, 1,0,3, 0,0,6,
            9,3,2, 0,0,0, 7,1,4
        },

        {
            7,2,3, 8,4,6, 1,5,9,
            6,1,5, 3,9,2, 4,7,8,
            8,4,9, 7,1,5, 6,3,2,

            3,7,8, 6,5,4, 9,2,1,
            1,9,4, 2,8,7, 3,6,5,
            2,5,6, 9,3,1, 8,4,7,

            5,6,1, 4,7,9, 2,8,3,
            4,8,7, 1,2,3, 5,9,6,
            9,3,2, 5,6,8, 7,1,4
        }
    },

    { // 2
        {
            0,0,7, 1,5,0, 9,0,0,
            0,0,9, 4,3,0, 0,0,0,
            5,0,0, 0,0,2, 0,1,3,

            0,0,6, 5,0,4, 0,2,9,
            4,3,0, 0,8,0, 0,5,7,
            9,7,0, 3,0,1, 4,0,0,

            7,6,0, 2,0,0, 0,0,5,
            0,0,0, 0,9,6, 2,0,0,
            0,0,3, 0,4,5, 6,0,0
        },

        {
            3,2,7, 1,5,8, 9,4,6,
            6,1,9, 4,3,7, 5,8,2,
            5,4,8, 9,6,2, 7,1,3,

            1,8,6, 5,7,4, 3,2,9,
            4,3,2, 6,8,9, 1,5,7,
            9,7,5, 3,2,1, 4,6,8,

            7,6,4, 2,1,3, 8,9,5,
            8,5,1, 7,9,6, 2,3,4,
            2,9,3, 8,4,5, 6,7,1
        }
    },

    { // 3
        {
            1,0,0, 0,0,0, 0,0,9,
            0,4,0, 2,6,1, 0,3,0,
            0,6,0, 0,5,0, 0,1,0,

            0,0,5, 6,0,3, 4,0,0,
            8,1,4, 7,0,5, 3,9,6,
            0,0,9, 0,1,0, 7,0,0,

            0,0,0, 9,3,4, 0,0,0,
            4,8,0, 5,7,2, 0,6,3,
            3,0,0, 0,0,0, 0,0,5
        },

        {
            1,5,3, 8,4,7, 6,2,9,
            9,4,8, 2,6,1, 5,3,7,
            7,6,2, 3,5,9, 8,1,4,

            2,7,5, 6,9,3, 4,8,1,
            8,1,4, 7,2,5, 3,9,6,
            6,3,9, 4,1,8, 7,5,2,

            5,2,6, 9,3,4, 1,7,8,
            4,8,1, 5,7,2, 9,6,3,
            3,9,7, 1,8,6, 2,4,5
        }
    },

    { // 4
        {
            3,2,1, 0,5,0, 9,4,7,
            7,8,0, 0,1,0, 0,6,5,
            0,0,6, 7,0,4, 1,0,0,

            5,4,9, 0,0,0, 7,8,6,
            0,0,0, 0,0,0, 0,0,0,
            0,0,0, 9,0,6, 0,0,0,

            1,0,5, 0,6,0, 4,0,2,
            0,3,0, 2,0,7, 0,5,0,
            2,0,7, 0,4,0, 8,0,3
        },

        {
            3,2,1, 6,5,8, 9,4,7,
            7,8,4, 3,1,9, 2,6,5,
            9,5,6, 7,2,4, 1,3,8,

            5,4,9, 1,3,2, 7,8,6,
            6,7,2, 4,8,5, 3,1,9,
            8,1,3, 9,7,6, 5,2,4,

            1,9,5, 8,6,3, 4,7,2,
            4,3,8, 2,9,7, 6,5,1,
            2,6,7, 5,4,1, 8,9,3
        }
    },

    { // 5
        {
            0,0,0, 6,9,0, 8,0,0,
            0,7,4, 0,8,1, 3,6,0,
            8,1,0, 7,0,5, 0,4,0,

            0,8,5, 0,0,0, 7,0,4,
            2,3,0, 0,0,0, 0,8,6,
            4,0,9, 0,0,0, 5,2,0,

            0,9,0, 5,0,3, 0,1,7,
            0,4,1, 9,7,0, 6,5,0,
            0,0,2, 0,4,6, 0,0,0
        },

        {
            5,2,3, 6,9,4, 8,7,1,
            9,7,4, 2,8,1, 3,6,5,
            8,1,6, 7,3,5, 2,4,9,

            1,8,5, 3,6,2, 7,9,4,
            2,3,7, 4,5,9, 1,8,6,
            4,6,9, 8,1,7, 5,2,3,

            6,9,8, 5,2,3, 4,1,7,
            3,4,1, 9,7,8, 6,5,2,
            7,5,2, 1,4,6, 9,3,8
        }
    },

    { // 6
        {
            0,7,0, 4,0,9, 0,1,0,
            0,0,0, 3,0,5, 0,0,0,
            6,0,0, 8,1,2, 0,0,3,

            0,6,7, 0,8,0, 9,4,0,
            0,0,0, 5,4,6, 0,0,0,
            1,4,3, 0,0,0, 5,6,8,

            0,0,5, 0,0,0, 4,0,0,
            8,3,0, 7,2,4, 0,5,9,
            4,0,9, 0,0,0, 2,0,7
        },

        {
            3,7,2, 4,6,9, 8,1,5,
            9,8,1, 3,7,5, 6,2,4,
            6,5,4, 8,1,2, 7,9,3,

            5,6,7, 1,8,3, 9,4,2,
            2,9,8, 5,4,6, 3,7,1,
            1,4,3, 2,9,7, 5,6,8,

            7,2,5, 9,3,1, 4,8,6,
            8,3,6, 7,2,4, 1,5,9,
            4,1,9, 6,5,8, 2,3,7
        }
    }
};

static SudokuTest::SudokuTestData data_medium = {
};

static SudokuTest::SudokuTestData data_hard = {
};

static SudokuTest::SudokuTestData data_extream = {
};

void SudokuTest::TestSudoku()
{
    TestSudokuEasy();
    TestSudokuMedium();
    TestSudokuHard();
    TestSudokuExtream();
}

void SudokuTest::TestSudokuEasy()
{
    TestSudokuLocalData(data_easy, "TestSudokuEasy"s);
}

void SudokuTest::TestSudokuMedium()
{
    TestSudokuLocalData(data_medium, "TestSudokuMedium"s);
}

void SudokuTest::TestSudokuHard()
{
    TestSudokuLocalData(data_hard, "TestSudokuHard"s);
}

void SudokuTest::TestSudokuExtream()
{
    TestSudokuLocalData(data_extream, "TestSudokuExtream"s);
}

void SudokuTest::TestSudokuLocalData(const SudokuTestData& data, std::string& test_name)
{
    for (const auto& [input_data, result_data] : data) {
        Sudoku input(input_data);
        Sudoku result(result_data);
        SudokuSolver solver(input);
        solver.Solve();
        assert(input == result);
    }
    std::cout << test_name << " Ok"s << std::endl;
}
