#pragma once

#include <string>
#include <vector>

class SudokuTest
{
public:
    using SudokuInput = std::vector<int>;
    using SudokuTestData = std::vector<std::pair<SudokuInput, SudokuInput>>;

    static void TestSudoku();

    static void TestSudokuEasy();
    static void TestSudokuMedium();
    static void TestSudokuHard();
    static void TestSudokuExtream();

private:
    static void TestSudokuLocalData(const SudokuTestData& data, const std::string& test_name);
};

