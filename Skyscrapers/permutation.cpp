#include "permutation.h"

#include "permutation/cluepair.h"
#include "permutation/permutations.h"
#include "permutation/slice.h"
#include "shared/board.h"
#include "shared/cluehints.h"
#include "shared/row.h"

#include <cassert>
#include <chrono>

namespace permutation {

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto clueHints = getClueHints(clues, boardSize);

    Board board{boardSize};

    board.insert(clueHints);
    board.insert(startingGrid);

    if (board.isSolved()) {
        return board.skyscrapers;
    }

    auto cluePairs = makeCluePairs(clues);
    Permutations permutations(boardSize, Span{&cluePairs[0], cluePairs.size()},
                              Span{&board.mRows[0], board.mRows.size()});

    std::vector<Slice> slices =
        makeSlices(permutations, board.mRows, cluePairs);

    for (;;) {
        bool allFull = true;
        for (std::size_t i = 0; i < slices.size(); ++i) {
            if (slices[i].isSolved()) {
                continue;
            }
            slices[i].solveFromPossiblePermutations();
            slices[i].guessSkyscraperOutOfNeighbourNopes();

            if (!slices[i].isSolved()) {
                allFull = false;
            }
        }

        if (allFull) {
            break;
        }
    }

    return board.skyscrapers;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace permutation
