#include "backtracking/tst_backtracking_partialtest.h"
#include "backtracking/tst_backtrackingtest.h"
#include "permutation/tst_permutation_partialtest.h"
#include "permutation/tst_permutationtest.h"
//#include "tst_codewarsbacktrackingtest.h"
//#include "tst_codewarspermutationtest.h"
//#include "tst_hybridtest.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
