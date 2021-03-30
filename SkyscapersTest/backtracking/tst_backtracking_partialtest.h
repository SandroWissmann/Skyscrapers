#ifndef TST_BACKTRACKING_BACKTRACKING_PARTIALTEST_H
#define TST_BACKTRACKING_BACKTRACKING_PARTIALTEST_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../test_skyscraper_partial_provider.h"

#include "../../Skyscrapers/backtracking.h"

#include <vector>

using namespace testing;

TEST(BacktrackingPartial, sky4_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky4_partial.clues, sky4_partial.board,
                                        sky4_partial.board.size()),
              sky4_partial.result);
}

TEST(BacktrackingPartial, sky4_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky4_partial_2.clues,
                                        sky4_partial_2.board,
                                        sky4_partial_2.board.size()),
              sky4_partial_2.result);
}

TEST(BacktrackingPartial, sky5_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky5_partial.clues, sky5_partial.board,
                                        sky5_partial.board.size()),
              sky5_partial.result);
}

TEST(BacktrackingPartial, sky5_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky5_partial_2.clues,
                                        sky5_partial_2.board,
                                        sky5_partial_2.board.size()),
              sky5_partial_2.result);
}

TEST(BacktrackingPartial, sky6_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky6_partial.clues, sky6_partial.board,
                                        sky6_partial.board.size()),
              sky6_partial.result);
}

TEST(BacktrackingPartial, sky6_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky6_partial_2.clues,
                                        sky6_partial_2.board,
                                        sky6_partial_2.board.size()),
              sky6_partial_2.result);
}

TEST(BacktrackingPartial, sky7_easy_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky7_easy_partial.clues,
                                        sky7_easy_partial.board,
                                        sky7_easy_partial.board.size()),
              sky7_easy_partial.result);
}

TEST(BacktrackingPartial, sky7_easy_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky7_easy_partial_2.clues,
                                        sky7_easy_partial_2.board,
                                        sky7_easy_partial_2.board.size()),
              sky7_easy_partial_2.result);
}

TEST(BacktrackingPartial, sky7_medium_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky7_medium_partial.clues,
                                        sky7_medium_partial.board,
                                        sky7_medium_partial.board.size()),
              sky7_medium_partial.result);
}

// ~1100ms
TEST(BacktrackingPartial, DISABLED_sky7_hard_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky7_hard_partial.clues,
                                        sky7_hard_partial.board,
                                        sky7_hard_partial.board.size()),
              sky7_hard_partial.result);
}

TEST(BacktrackingPartial, sky8_easy_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky8_easy_partial.clues,
                                        sky8_easy_partial.board,
                                        sky8_easy_partial.board.size()),
              sky8_easy_partial.result);
}

TEST(BacktrackingPartial, sky8_medium_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky8_medium_partial.clues,
                                        sky8_medium_partial.board,
                                        sky8_medium_partial.board.size()),
              sky8_medium_partial.result);
}

TEST(BacktrackingPartial, DISABLED_sky8_hard_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky8_hard_partial.clues,
                                        sky8_hard_partial.board,
                                        sky8_hard_partial.board.size()),
              sky8_hard_partial.result);
}

TEST(BacktrackingPartial, sky9_easy_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky9_easy_partial.clues,
                                        sky9_easy_partial.board,
                                        sky9_easy_partial.board.size()),
              sky9_easy_partial.result);
}

TEST(BacktrackingPartial, sky9_easy_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky9_easy_partial_2.clues,
                                        sky9_easy_partial_2.board,
                                        sky9_easy_partial_2.board.size()),
              sky9_easy_partial_2.result);
}

TEST(BacktrackingPartial, sky10_easy_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky10_easy_partial.clues,
                                        sky10_easy_partial.board,
                                        sky10_easy_partial.board.size()),
              sky10_easy_partial.result);
}

TEST(BacktrackingPartial, sky10_easy_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky10_easy_partial_2.clues,
                                        sky10_easy_partial_2.board,
                                        sky10_easy_partial_2.board.size()),
              sky10_easy_partial_2.result);
}

TEST(BacktrackingPartial, sky11_easy_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky11_easy_partial.clues,
                                        sky11_easy_partial.board,
                                        sky11_easy_partial.board.size()),
              sky11_easy_partial.result);
}

// ~3300ms
TEST(BacktrackingPartial, sky11_medium_partial)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky11_medium_partial.clues,
                                        sky11_medium_partial.board,
                                        sky11_medium_partial.board.size()),
              sky11_medium_partial.result);
}

// takes forever
TEST(BacktrackingPartial, DISABLED_sky11_medium_partial_2)
{
    EXPECT_EQ(backtracking::SolvePuzzle(sky11_medium_partial_2.clues,
                                        sky11_medium_partial_2.board,
                                        sky11_medium_partial_2.board.size()),
              sky11_medium_partial_2.result);
}

#endif // TST_BACKTRACKINGTEST_H
