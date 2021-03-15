#include "tst_backtrackingtest.h"
#include "tst_codewarsbacktrackingtest.h"
#include "tst_codewarspermutationtest.h"
#include "tst_permutationtest.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
