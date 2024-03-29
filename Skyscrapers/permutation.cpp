﻿#include "permutation.h"

#include "permutation/cluepair.h"
#include "permutation/permutations.h"
#include "permutation/slice.h"
#include "shared/board.h"
#include "shared/row.h"
#include "shared/rowclues.h"

#include <cassert>
#include <chrono>

namespace permutation {

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto rowClues = getRowClues(clues, boardSize);

    Board board{boardSize};

    board.insert(rowClues);
    board.insert(startingGrid);

    if (board.isSolved()) {
        return board.skyscrapers2d();
    }

    solveBoard(board, clues);

    return board.skyscrapers2d();
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

void solveBoard(Board &board, const std::vector<int> &clues)
{
    auto cluePairs = makeCluePairs(clues);
    Permutations permutations(board.size(),
                              Span{&cluePairs[0], cluePairs.size()},
                              Span{&board.mRows[0], board.mRows.size()});

    std::vector<Slice> slices =
        makeSlices(permutations, board.mRows, cluePairs, board.size());

    for (;;) {
        bool allFull = true;
        for (std::size_t i = 0; i < slices.size(); ++i) {
            if (slices[i].isSolved()) {
                continue;
            }
            slices[i].solveFromPossiblePermutations(board.size());
            slices[i].guessSkyscraperOutOfNeighbourNopes();

            if (!slices[i].isSolved()) {
                allFull = false;
            }
        }

        if (allFull) {
            break;
        }
    }
}

} // namespace permutation
