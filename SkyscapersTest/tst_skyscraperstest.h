﻿#ifndef TST_SKYSCRAPERETEST_H
#define TST_SKYSCRAPERETEST_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../Skyscrapers/skyscrapers.h"

#include <vector>

using namespace testing;

TEST(Skyscrapers, sky4_easy)
{
    std::vector<int> clues = {2, 2, 1, 3, 2, 2, 3, 1, 1, 2, 2, 3, 3, 2, 1, 3};

    std::vector<std::vector<int>> expectedResult = {
        {1, 3, 4, 2}, {4, 2, 1, 3}, {3, 4, 2, 1}, {2, 1, 3, 4}};

    auto result = SolvePuzzle(clues);

    EXPECT_EQ(result, expectedResult);
}

TEST(Skyscrapers, sky4_easy_2)
{
    std::vector<int> clues = {4, 2, 3, 1, 1, 3, 2, 2, 2, 2, 2, 1, 1, 2, 2, 3};

    std::vector<std::vector<int>> expectedResult = {
        {1, 3, 2, 4}, {2, 4, 3, 1}, {3, 1, 4, 2}, {4, 2, 1, 3}};

    auto result = SolvePuzzle(clues);

    EXPECT_EQ(result, expectedResult);
}

TEST(Skyscrapers, sky4_hard)
{
    std::vector<int> clues = {0, 0, 1, 2, 0, 2, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0};

    std::vector<std::vector<int>> expectedResult = {
        {2, 1, 4, 3}, {3, 4, 1, 2}, {4, 2, 3, 1}, {1, 3, 2, 4}};

    auto result = SolvePuzzle(clues);

    EXPECT_EQ(result, expectedResult);
}

TEST(Skyscrapers, sky4_hard_2)
{
    std::vector<int> clues = {0, 3, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0};

    std::vector<std::vector<int>> expectedResult = {
        {1, 2, 3, 4}, {3, 1, 4, 2}, {4, 3, 2, 1}, {2, 4, 1, 3}};

    auto result = SolvePuzzle(clues);

    EXPECT_EQ(result, expectedResult);
}

TEST(Skyscrapers, sky6_easy)
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

TEST(Skyscrapers, sky6_medium)
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

TEST(Skyscrapers, sky6_hard)
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

TEST(Skyscrapers, sky6_hard_2)
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

TEST(Skyscrapers, sky6_random)
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

TEST(Skyscrapers, sky6_random_2)
{
    std::vector<int> clues = {4, 1, 0, 0, 3, 0, 0, 2, 1, 0, 6, 0,
                              2, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0};

    std::vector<std::vector<int>> expectedResult = {{{2, 6, 1, 5, 3, 4},
                                                     {3, 4, 6, 2, 1, 5},
                                                     {4, 2, 3, 1, 5, 6},
                                                     {1, 3, 5, 6, 4, 2},
                                                     {6, 5, 4, 3, 2, 1},
                                                     {5, 1, 2, 4, 6, 3}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, sky6_random_3)
{
    std::vector<int> clues = {3, 0, 0, 0, 0, 0, 0, 2, 1, 0, 2, 4,
                              0, 0, 3, 0, 1, 0, 0, 0, 2, 4, 0, 5};

    std::vector<std::vector<int>> expectedResult = {{{1, 2, 3, 5, 6, 4},
                                                     {4, 5, 1, 6, 2, 3},
                                                     {2, 4, 5, 3, 1, 6},
                                                     {3, 1, 6, 4, 5, 2},
                                                     {6, 3, 2, 1, 4, 5},
                                                     {5, 6, 4, 2, 3, 1}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, sky7_medium)
{
    std::vector<int> clues = {7, 0, 0, 0, 2, 2, 3, 0, 0, 3, 0, 0, 0, 0,
                              3, 0, 3, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 4};

    std::vector<std::vector<int>> expectedResult = {{{1, 5, 6, 7, 4, 3, 2},
                                                     {2, 7, 4, 5, 3, 1, 6},
                                                     {3, 4, 5, 6, 7, 2, 1},
                                                     {4, 6, 3, 1, 2, 7, 5},
                                                     {5, 3, 1, 2, 6, 4, 7},
                                                     {6, 2, 7, 3, 1, 5, 4},
                                                     {7, 1, 2, 4, 5, 6, 3}}};

    auto result = SolvePuzzle(clues);

    EXPECT_EQ(result, expectedResult);
}

TEST(Skyscrapers, sky7_hard)
{
    std::vector<int> clues = {0, 2, 3, 0, 2, 0, 0, 5, 0, 4, 5, 0, 4, 0,
                              0, 4, 2, 0, 0, 0, 6, 5, 2, 2, 2, 2, 4, 1};

    std::vector<std::vector<int>> expectedResult = {{{7, 6, 2, 1, 5, 4, 3},
                                                     {1, 3, 5, 4, 2, 7, 6},
                                                     {6, 5, 4, 7, 3, 2, 1},
                                                     {5, 1, 7, 6, 4, 3, 2},
                                                     {4, 2, 1, 3, 7, 6, 5},
                                                     {3, 7, 6, 2, 1, 5, 4},
                                                     {2, 4, 3, 5, 6, 1, 7}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, sky7_very_hard)
{
    std::vector<int> clues = {0, 0, 5, 0, 0, 0, 6, 4, 0, 0, 2, 0, 2, 0,
                              0, 5, 2, 0, 0, 0, 5, 0, 3, 0, 5, 0, 0, 3};

    std::vector<std::vector<int>> expectedResult = {{{3, 4, 1, 7, 6, 5, 2},
                                                     {7, 1, 2, 5, 4, 6, 3},
                                                     {6, 3, 5, 2, 1, 7, 4},
                                                     {1, 2, 3, 6, 7, 4, 5},
                                                     {5, 7, 6, 4, 2, 3, 1},
                                                     {4, 5, 7, 1, 3, 2, 6},
                                                     {2, 6, 4, 3, 5, 1, 7}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, sky7_random)
{
    std::vector<int> clues = {0, 5, 0, 5, 0, 2, 0, 0, 0, 0, 4, 0, 0, 3,
                              6, 4, 0, 2, 0, 0, 3, 0, 3, 3, 3, 0, 0, 4};

    std::vector<std::vector<int>> expectedResult = {{{2, 3, 6, 1, 4, 5, 7},
                                                     {7, 1, 5, 2, 3, 4, 6},
                                                     {6, 4, 2, 3, 1, 7, 5},
                                                     {4, 5, 7, 6, 2, 3, 1},
                                                     {3, 2, 1, 5, 7, 6, 4},
                                                     {1, 6, 4, 7, 5, 2, 3},
                                                     {5, 7, 3, 4, 6, 1, 2}}};

    EXPECT_EQ(SolvePuzzle(clues), expectedResult);
}

TEST(Skyscrapers, sky4_partial)
{
    std::vector<int> clues = {2, 2, 1, 3, 2, 3, 1, 2, 2, 3, 3, 1, 1, 3, 2, 2};

    std::vector<std::vector<int>> startingBoard{
        {0, 0, 0, 1}, {0, 4, 0, 0}, {2, 0, 0, 0}, {0, 0, 2, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {3, 2, 4, 1}, {1, 4, 3, 2}, {2, 3, 1, 4}, {4, 1, 2, 3}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky4_partial_2)
{
    std::vector<int> clues = {3, 1, 2, 2, 3, 1, 2, 2, 3, 1, 2, 2, 3, 1, 2, 2};

    std::vector<std::vector<int>> startingBoard{
        {0, 0, 0, 1}, {0, 2, 0, 0}, {4, 0, 0, 0}, {0, 0, 4, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {2, 4, 3, 1}, {3, 2, 1, 4}, {4, 1, 2, 3}, {1, 3, 4, 2}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky5_partial)
{
    std::vector<int> clues = {4, 1, 4, 2, 3, 3, 2, 1, 3, 4,
                              2, 3, 2, 2, 1, 1, 2, 4, 2, 2};

    std::vector<std::vector<int>> startingBoard{{0, 0, 0, 4, 3},
                                                {0, 0, 2, 0, 0},
                                                {1, 0, 0, 0, 5},
                                                {0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {{2, 5, 1, 4, 3},
                                                    {3, 1, 2, 5, 4},
                                                    {1, 3, 4, 2, 5},
                                                    {4, 2, 5, 3, 1},
                                                    {5, 4, 3, 1, 2}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky5_partial_2)
{
    std::vector<int> clues = {3, 1, 3, 2, 2, 2, 1, 2, 3, 3,
                              3, 3, 2, 3, 1, 1, 3, 3, 2, 2};

    std::vector<std::vector<int>> startingBoard{{0, 0, 0, 3, 4},
                                                {0, 0, 3, 0, 0},
                                                {3, 0, 0, 0, 1},
                                                {0, 0, 0, 0, 0},
                                                {0, 0, 0, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {{2, 5, 1, 3, 4},
                                                    {4, 1, 3, 2, 5},
                                                    {3, 4, 2, 5, 1},
                                                    {1, 2, 5, 4, 3},
                                                    {5, 3, 4, 1, 2}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky6_partial)
{
    std::vector<int> clues = {2, 2, 3, 4, 2, 1, 1, 2, 3, 3, 4, 2,
                              5, 4, 1, 3, 2, 2, 3, 2, 1, 2, 4, 2};

    std::vector<std::vector<int>> startingBoard{
        {5, 0, 0, 0, 3, 0}, {2, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 4, 3}, {0, 0, 0, 0, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {5, 4, 1, 2, 3, 6}, {2, 3, 4, 1, 6, 5}, {3, 2, 6, 4, 5, 1},
        {6, 1, 5, 3, 2, 4}, {1, 6, 2, 5, 4, 3}, {4, 5, 3, 6, 1, 2}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky6_partial_2)
{
    std::vector<int> clues = {2, 2, 3, 4, 2, 1, 1, 2, 3, 3, 4, 2,
                              5, 4, 1, 3, 2, 2, 3, 2, 1, 2, 4, 2};

    std::vector<std::vector<int>> startingBoard{
        {5, 0, 0, 0, 3, 0}, {2, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 4, 3}, {0, 0, 0, 0, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {5, 4, 1, 2, 3, 6}, {2, 3, 4, 1, 6, 5}, {3, 2, 6, 4, 5, 1},
        {6, 1, 5, 3, 2, 4}, {1, 6, 2, 5, 4, 3}, {4, 5, 3, 6, 1, 2}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky7_partial)
{
    std::vector<int> clues = {3, 1, 2, 3, 4, 4, 2, 5, 1, 4, 3, 3, 2, 2,
                              2, 2, 2, 2, 4, 3, 1, 1, 3, 3, 2, 3, 4, 2};

    std::vector<std::vector<int>> startingBoard{
        {0, 0, 0, 0, 0, 3, 0}, {0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 0, 0}, {0, 0, 0, 7, 0, 0, 0}, {4, 0, 0, 2, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {5, 7, 6, 4, 2, 3, 1}, {3, 1, 5, 6, 4, 2, 7}, {2, 3, 7, 1, 6, 5, 4},
        {6, 2, 1, 5, 7, 4, 3}, {1, 5, 4, 7, 3, 6, 2}, {4, 6, 3, 2, 1, 7, 5},
        {7, 4, 2, 3, 5, 1, 6}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky7_partial_2)
{
    std::vector<int> clues = {3, 1, 2, 3, 4, 4, 2, 5, 1, 4, 3, 3, 2, 2,
                              2, 2, 2, 2, 4, 3, 1, 1, 3, 3, 2, 3, 4, 2};

    std::vector<std::vector<int>> startingBoard{
        {0, 0, 0, 0, 0, 3, 0}, {0, 1, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 0, 0}, {0, 0, 0, 7, 0, 0, 0}, {4, 0, 0, 2, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {5, 7, 6, 4, 2, 3, 1}, {3, 1, 5, 6, 4, 2, 7}, {2, 3, 7, 1, 6, 5, 4},
        {6, 2, 1, 5, 7, 4, 3}, {1, 5, 4, 7, 3, 6, 2}, {4, 6, 3, 2, 1, 7, 5},
        {7, 4, 2, 3, 5, 1, 6}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky8_easy_partial)
{
    std::vector<int> clues = {1, 5, 2, 2, 3, 2, 3, 4, 3, 3, 3, 2, 3, 4, 2, 1,
                              1, 3, 2, 2, 4, 5, 3, 2, 2, 4, 2, 3, 3, 4, 2, 1};

    std::vector<std::vector<int>> startingBoard{
        {0, 0, 0, 4, 0, 0, 0, 0}, {0, 0, 0, 0, 3, 0, 7, 0},
        {3, 0, 0, 0, 0, 0, 0, 2}, {0, 1, 0, 0, 0, 0, 0, 3},
        {0, 0, 6, 0, 0, 8, 5, 0}, {0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 2, 0, 0, 0, 0, 0}, {0, 2, 0, 0, 0, 0, 4, 0},
    };

    std::vector<std::vector<int>> expectedResult = {
        {8, 3, 1, 4, 6, 7, 2, 5}, {5, 4, 8, 2, 3, 1, 7, 6},
        {3, 6, 7, 8, 4, 5, 1, 2}, {6, 1, 4, 7, 5, 2, 8, 3},
        {4, 7, 6, 3, 2, 8, 5, 1}, {2, 8, 5, 1, 7, 3, 6, 4},
        {1, 5, 2, 6, 8, 4, 3, 7}, {7, 2, 3, 5, 1, 6, 4, 8},
    };

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky8_medium_partial)
{
    std::vector<int> clues = {3, 3, 3, 2, 1, 4, 3, 3, 3, 4, 2, 1, 3, 3, 3, 2,
                              4, 1, 2, 5, 2, 3, 2, 3, 3, 2, 2, 1, 4, 2, 2, 3};

    std::vector<std::vector<int>> startingBoard{
        {0, 4, 0, 0, 0, 0, 0, 0}, {0, 3, 0, 0, 0, 0, 2, 0},
        {3, 0, 0, 0, 0, 5, 0, 0}, {0, 6, 0, 3, 0, 0, 0, 0},
        {0, 0, 0, 0, 7, 0, 0, 0}, {1, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 4, 0, 0, 7, 5}, {0, 0, 0, 0, 0, 4, 0, 0}};

    std::vector<std::vector<int>> expectedResult = {
        {5, 4, 3, 7, 8, 1, 6, 2}, {7, 3, 4, 8, 5, 6, 2, 1},
        {3, 2, 8, 1, 6, 5, 4, 7}, {4, 6, 1, 3, 2, 7, 5, 8},
        {8, 5, 6, 2, 7, 3, 1, 4}, {1, 8, 7, 5, 4, 2, 3, 6},
        {6, 1, 2, 4, 3, 8, 7, 5}, {2, 7, 5, 6, 1, 4, 8, 3}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

TEST(Skyscrapers, sky8_hard_partial)
{
    std::vector<int> clues = {3, 0, 4, 3, 3, 0, 3, 2, 0, 0, 2, 4, 0, 4, 1, 5,
                              0, 4, 0, 3, 0, 0, 4, 3, 0, 0, 0, 3, 3, 4, 0, 3};

    std::vector<std::vector<int>> startingBoard{
        {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 5, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0, 0, 4},
        {0, 0, 1, 6, 5, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 1}};

    std::vector<std::vector<int>> expectedResult = {
        {1, 6, 2, 4, 3, 8, 5, 7}, {6, 8, 5, 2, 4, 1, 7, 3},
        {2, 3, 7, 5, 8, 4, 1, 6}, {3, 1, 6, 8, 7, 5, 2, 4},
        {4, 7, 1, 6, 5, 3, 8, 2}, {8, 2, 4, 3, 1, 7, 6, 5},
        {7, 5, 3, 1, 2, 6, 4, 8}, {5, 4, 8, 7, 6, 2, 3, 1}};

    EXPECT_EQ(SolvePuzzle(clues, startingBoard, startingBoard.size()),
              expectedResult);
}

#endif // TST_SKYSCRAPERETEST_H
