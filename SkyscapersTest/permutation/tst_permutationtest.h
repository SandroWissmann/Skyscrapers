#ifndef TST_PERMUTATION_PERMUTATIONTEST_H
#define TST_PERMUTATION_PERMUTATIONTEST_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../test_skyscraper_provider.h"

#include "../../Skyscrapers/permutation.h"

#include <vector>

using namespace testing;

TEST(Permutation, sky4_easy)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky4_easy.clues), sky4_easy.result);
}

TEST(Permutation, sky4_easy_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky4_easy_2.clues), sky4_easy_2.result);
}

TEST(Permutation, sky4_hard)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky4_hard.clues), sky4_hard.result);
}

TEST(Permutation, sky4_hard_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky4_hard_2.clues), sky4_hard_2.result);
}

TEST(Permutation, sky6_easy)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_easy.clues), sky6_easy.result);
}

TEST(Permutation, sky6_medium)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_medium.clues), sky6_medium.result);
}

TEST(Permutation, sky6_hard)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_hard.clues), sky6_hard.result);
}

TEST(Permutation, sky6_hard_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_hard_2.clues), sky6_hard_2.result);
}

TEST(Permutation, sky6_random)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_random.clues), sky6_random.result);
}

TEST(Permutation, sky6_random_2)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_random_2.clues),
              sky6_random_2.result);
}

TEST(Permutation, sky6_random_3)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky6_random_3.clues),
              sky6_random_3.result);
}

TEST(Permutation, sky7_medium)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_medium.clues), sky7_medium.result);
}

TEST(Permutation, sky7_hard)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_hard.clues), sky7_hard.result);
}

TEST(Permutation, sky7_very_hard)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_very_hard.clues),
              sky7_very_hard.result);
}

TEST(Permutation, sky7_random)
{
    EXPECT_EQ(permutation::SolvePuzzle(sky7_random.clues), sky7_random.result);
}

#endif // TST_PERMUTATION_PERMUTATIONTEST_H
