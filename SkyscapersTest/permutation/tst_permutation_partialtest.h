#ifndef TST_PERMUTATION_PERMUTATION_PARTIALTEST_H
#define TST_PERMUTATION_PERMUTATION_PARTIALTEST_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../test_skyscraper_partial_provider.h"

#include "../../Skyscrapers/permutation.h"

#include <vector>

using namespace testing;

TEST(PermutationPartial, sky4_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky4_partial.clues, sky4_partial.board,
                                       sky4_partial.board.size()),
              sky4_partial.result);
}

TEST(PermutationPartial, sky4_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky4_partial_2.clues,
                                       sky4_partial_2.board,
                                       sky4_partial_2.board.size()),
              sky4_partial_2.result);
}

TEST(PermutationPartial, sky5_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky5_partial.clues, sky5_partial.board,
                                       sky5_partial.board.size()),
              sky5_partial.result);
}

TEST(PermutationPartial, sky5_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky5_partial_2.clues,
                                       sky5_partial_2.board,
                                       sky5_partial_2.board.size()),
              sky5_partial_2.result);
}

TEST(PermutationPartial, sky6_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_partial.clues, sky6_partial.board,
                                       sky6_partial.board.size()),
              sky6_partial.result);
}

TEST(PermutationPartial, sky6_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_partial_2.clues,
                                       sky6_partial_2.board,
                                       sky6_partial_2.board.size()),
              sky6_partial_2.result);
}

TEST(PermutationPartial, sky7_easy_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_easy_partial.clues,
                                       sky7_easy_partial.board,
                                       sky7_easy_partial.board.size()),
              sky7_easy_partial.result);
}

// ??? takes too long why?
TEST(PermutationPartial, DISABLED_sky7_easy_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_easy_partial_2.clues,
                                       sky7_easy_partial_2.board,
                                       sky7_easy_partial_2.board.size()),
              sky7_easy_partial_2.result);
}

TEST(PermutationPartial, sky7_medium_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_medium_partial.clues,
                                       sky7_medium_partial.board,
                                       sky7_medium_partial.board.size()),
              sky7_medium_partial.result);
}

TEST(PermutationPartial, sky7_hard_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_hard_partial.clues,
                                       sky7_hard_partial.board,
                                       sky7_hard_partial.board.size()),
              sky7_hard_partial.result);
}

TEST(PermutationPartial, sky8_easy_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky8_easy_partial.clues,
                                       sky8_easy_partial.board,
                                       sky8_easy_partial.board.size()),
              sky8_easy_partial.result);
}

TEST(PermutationPartial, sky8_medium_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky8_medium_partial.clues,
                                       sky8_medium_partial.board,
                                       sky8_medium_partial.board.size()),
              sky8_medium_partial.result);
}

TEST(PermutationPartial, sky8_hard_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky8_hard_partial.clues,
                                       sky8_hard_partial.board,
                                       sky8_hard_partial.board.size()),
              sky8_hard_partial.result);
}

TEST(PermutationPartial, sky9_easy_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky9_easy_partial.clues,
                                       sky9_easy_partial.board,
                                       sky9_easy_partial.board.size()),
              sky9_easy_partial.result);
}

TEST(PermutationPartial, sky9_easy_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky9_easy_partial_2.clues,
                                       sky9_easy_partial_2.board,
                                       sky9_easy_partial_2.board.size()),
              sky9_easy_partial_2.result);
}

// ~500
TEST(PermutationPartial, sky10_easy_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky10_easy_partial.clues,
                                       sky10_easy_partial.board,
                                       sky10_easy_partial.board.size()),
              sky10_easy_partial.result);
}

// ~500
TEST(PermutationPartial, sky10_easy_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky10_easy_partial_2.clues,
                                       sky10_easy_partial_2.board,
                                       sky10_easy_partial_2.board.size()),
              sky10_easy_partial_2.result);
}

// ~6300
TEST(PermutationPartial, sky11_easy_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky11_easy_partial.clues,
                                       sky11_easy_partial.board,
                                       sky11_easy_partial.board.size()),
              sky11_easy_partial.result);
}

// ~6700
TEST(PermutationPartial, sky11_medium_partial)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky11_medium_partial.clues,
                                       sky11_medium_partial.board,
                                       sky11_medium_partial.board.size()),
              sky11_medium_partial.result);
}

// ~7500
TEST(PermutationPartial, sky11_medium_partial_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky11_medium_partial_2.clues,
                                       sky11_medium_partial_2.board,
                                       sky11_medium_partial_2.board.size()),
              sky11_medium_partial_2.result);
}

#endif // TST_PERMUTATION_PERMUTATION_PARTIALTEST_H
