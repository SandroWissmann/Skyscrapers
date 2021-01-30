#ifndef TST_SKYSCRAPERETEST_H
#define TST_SKYSCRAPERETEST_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../Skyscrapers/skyscrapers.h"

#include <vector>

using namespace testing;

TEST(Skyscrapers, easy)
{
    std::vector<int> clues = {3, 2, 2, 3, 2, 1, 1, 2, 3, 3, 2, 2,
                              5, 1, 2, 2, 4, 3, 3, 2, 1, 2, 2, 4};

    std::vector<std::vector<int>> expectedResult = {{{2, 1, 4, 3, 5, 6},
                                                     {1, 6, 3, 2, 4, 5},
                                                     {4, 3, 6, 5, 1, 2},
                                                     {6, 5, 2, 1, 3, 4},
                                                     {5, 4, 1, 6, 2, 3},
                                                     {3, 2, 5, 4, 6, 1}}};

    auto result = SolvePuzzle(clues);

    EXPECT_EQ(result, expectedResult);
}

TEST(Skyscrapers, medium)
{
    std::vector<int> clues = {0, 0, 0, 2, 2, 0, 0, 0, 0, 6, 3, 0,
                              0, 4, 0, 0, 0, 0, 4, 4, 0, 3, 0, 0};

    std::vector<std::vector<int>> expectedResult = {{{5, 6, 1, 4, 3, 2},
                                                     {4, 1, 3, 2, 6, 5},
                                                     {2, 3, 6, 1, 5, 4},
                                                     {6, 5, 4, 3, 2, 1},
                                                     {1, 2, 5, 6, 4, 3},
                                                     {3, 4, 2, 5, 1, 6}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, hard)
{
    std::vector<int> clues = {0, 3, 0, 5, 3, 4, 0, 0, 0, 0, 0, 1,
                              0, 3, 0, 3, 2, 3, 3, 2, 0, 3, 1, 0};

    std::vector<std::vector<int>> expectedResult = {{{5, 2, 6, 1, 4, 3},
                                                     {6, 4, 3, 2, 5, 1},
                                                     {3, 1, 5, 4, 6, 2},
                                                     {2, 6, 1, 5, 3, 4},
                                                     {4, 3, 2, 6, 1, 5},
                                                     {1, 5, 4, 3, 2, 6}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, hard_2)
{
    std::vector<int> clues = {4, 3, 2, 5, 1, 5, 2, 2, 2, 2, 3, 1,
                              1, 3, 2, 3, 3, 3, 5, 4, 1, 2, 3, 4};

    std::vector<std::vector<int>> expectedResult = {{{3, 4, 5, 1, 6, 2},
                                                     {4, 5, 6, 2, 1, 3},
                                                     {5, 6, 1, 3, 2, 4},
                                                     {6, 1, 2, 4, 3, 5},
                                                     {2, 3, 4, 6, 5, 1},
                                                     {1, 2, 3, 5, 4, 6}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, random)
{
    std::vector<int> clues = {3, 2, 1, 2, 2, 4, 3, 2, 2, 3, 2, 1,
                              1, 2, 3, 3, 2, 2, 5, 1, 2, 2, 4, 3};

    std::vector<std::vector<int>> expectedResult = {{{3, 5, 6, 4, 1, 2},
                                                     {2, 4, 5, 3, 6, 1},
                                                     {5, 1, 2, 6, 3, 4},
                                                     {4, 6, 1, 5, 2, 3},
                                                     {6, 2, 3, 1, 4, 5},
                                                     {1, 3, 4, 2, 5, 6}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

#endif // TST_SKYSCRAPERETEST_H
