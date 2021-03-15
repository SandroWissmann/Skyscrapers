#ifndef TST_CODEWARSBACKTRACKINGTEST_H
#define TST_CODEWARSBACKTRACKINGTEST_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "testdataprovider.h"

#include "../Skyscrapers/codewarsbacktracking.h"

#include <vector>

using namespace testing;

TEST(CodewarsBacktracking, sky4_easy)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky4_easy.clues),
              sky4_easy.result);
}

TEST(CodewarsBacktracking, sky4_easy_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky4_easy_2.clues),
              sky4_easy_2.result);
}

TEST(CodewarsBacktracking, sky4_hard)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky4_hard.clues),
              sky4_hard.result);
}

TEST(CodewarsBacktracking, sky4_hard_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky4_hard_2.clues),
              sky4_hard_2.result);
}

TEST(CodewarsBacktracking, sky6_easy)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_easy.clues),
              sky6_easy.result);
}

TEST(CodewarsBacktracking, sky6_medium)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_medium.clues),
              sky6_medium.result);
}

TEST(CodewarsBacktracking, sky6_hard)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_hard.clues),
              sky6_hard.result);
}

TEST(CodewarsBacktracking, sky6_hard_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_hard_2.clues),
              sky6_hard_2.result);
}

TEST(CodewarsBacktracking, sky6_random)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_random.clues),
              sky6_random.result);
}

TEST(CodewarsBacktracking, sky6_random_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_random_2.clues),
              sky6_random_2.result);
}

TEST(CodewarsBacktracking, sky6_random_3)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_random_3.clues),
              sky6_random_3.result);
}

TEST(CodewarsBacktracking, sky7_medium)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky7_medium.clues),
              sky7_medium.result);
}

TEST(CodewarsBacktracking, sky7_hard)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky7_hard.clues),
              sky7_hard.result);
}

TEST(CodewarsBacktracking, sky7_very_hard)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky7_very_hard.clues),
              sky7_very_hard.result);
}

TEST(CodewarsBacktracking, sky7_random)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky7_random.clues),
              sky7_random.result);
}

TEST(CodewarsBacktracking, sky4_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky4_partial.clues,
                                                sky4_partial.board,
                                                sky4_partial.board.size()),
              sky4_partial.result);
}

TEST(CodewarsBacktracking, sky4_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky4_partial_2.clues,
                                                sky4_partial_2.board,
                                                sky4_partial_2.board.size()),
              sky4_partial_2.result);
}

TEST(CodewarsBacktracking, sky5_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky5_partial.clues,
                                                sky5_partial.board,
                                                sky5_partial.board.size()),
              sky5_partial.result);
}

TEST(CodewarsBacktracking, sky5_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky5_partial_2.clues,
                                                sky5_partial_2.board,
                                                sky5_partial_2.board.size()),
              sky5_partial_2.result);
}

TEST(CodewarsBacktracking, sky6_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_partial.clues,
                                                sky6_partial.board,
                                                sky6_partial.board.size()),
              sky6_partial.result);
}

TEST(CodewarsBacktracking, sky6_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky6_partial_2.clues,
                                                sky6_partial_2.board,
                                                sky6_partial_2.board.size()),
              sky6_partial_2.result);
}

TEST(CodewarsBacktracking, sky7_easy_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky7_easy_partial.clues,
                                                sky7_easy_partial.board,
                                                sky7_easy_partial.board.size()),
              sky7_easy_partial.result);
}

TEST(CodewarsBacktracking, sky7_easy_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky7_easy_partial_2.clues, sky7_easy_partial_2.board,
                  sky7_easy_partial_2.board.size()),
              sky7_easy_partial_2.result);
}

TEST(CodewarsBacktracking, sky7_medium_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky7_medium_partial.clues, sky7_medium_partial.board,
                  sky7_medium_partial.board.size()),
              sky7_medium_partial.result);
}

TEST(CodewarsBacktracking, sky7_hard_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky7_hard_partial.clues,
                                                sky7_hard_partial.board,
                                                sky7_hard_partial.board.size()),
              sky7_hard_partial.result);
}

TEST(CodewarsBacktracking, sky8_easy_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky8_easy_partial.clues,
                                                sky8_easy_partial.board,
                                                sky8_easy_partial.board.size()),
              sky8_easy_partial.result);
}

TEST(CodewarsBacktracking, sky8_medium_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky8_medium_partial.clues, sky8_medium_partial.board,
                  sky8_medium_partial.board.size()),
              sky8_medium_partial.result);
}

TEST(CodewarsBacktracking, sky8_hard_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky8_hard_partial.clues,
                                                sky8_hard_partial.board,
                                                sky8_hard_partial.board.size()),
              sky8_hard_partial.result);
}

TEST(CodewarsBacktracking, sky9_easy_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(sky9_easy_partial.clues,
                                                sky9_easy_partial.board,
                                                sky9_easy_partial.board.size()),
              sky9_easy_partial.result);
}

TEST(CodewarsBacktracking, sky9_easy_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky9_easy_partial_2.clues, sky9_easy_partial_2.board,
                  sky9_easy_partial_2.board.size()),
              sky9_easy_partial_2.result);
}

TEST(CodewarsBacktracking, sky10_easy_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky10_easy_partial.clues, sky10_easy_partial.board,
                  sky10_easy_partial.board.size()),
              sky10_easy_partial.result);
}

TEST(CodewarsBacktracking, sky10_easy_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky10_easy_partial_2.clues, sky10_easy_partial_2.board,
                  sky10_easy_partial_2.board.size()),
              sky10_easy_partial_2.result);
}

TEST(CodewarsBacktracking, sky11_easy_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky11_easy_partial.clues, sky11_easy_partial.board,
                  sky11_easy_partial.board.size()),
              sky11_easy_partial.result);
}

TEST(CodewarsBacktracking, sky11_medium_partial)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky11_medium_partial.clues, sky11_medium_partial.board,
                  sky11_medium_partial.board.size()),
              sky11_medium_partial.result);
}

TEST(CodewarsBacktracking, sky11_medium_partial_2)
{
    EXPECT_EQ(codewarsbacktracking::SolvePuzzle(
                  sky11_medium_partial_2.clues, sky11_medium_partial_2.board,
                  sky11_medium_partial_2.board.size()),
              sky11_medium_partial_2.result);
}

#endif // TST_BACKTRACKINGTEST_H
