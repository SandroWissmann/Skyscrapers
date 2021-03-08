#include "permutation.h"

#include "permutation/board.h"
#include "permutation/cluepair.h"
#include "permutation/field.h"
#include "permutation/permutations.h"
#include "permutation/row.h"
#include "permutation/slice.h"

#include <cassert>
#include <chrono>

namespace permutation {

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    auto cluePairs = makeCluePairs(clues);

    int boardSize = clues.size() / 4;
    Board board{boardSize};

    auto fields = makeFields(board.skyscrapers, board.nopes);

    auto rows = makeRows(fields);

    connectRowsWithCrossingRows(rows);

    if (!startingGrid.empty()) {
        insertExistingSkyscrapersFromStartingGrid(rows, startingGrid);
    }

    //    auto t1 = std::chrono::high_resolution_clock::now();

    Permutations permutations(boardSize, Span{&cluePairs[0], cluePairs.size()},
                              Span{&rows[0], rows.size()});

    //    auto t2 = std::chrono::high_resolution_clock::now();
    //    std::cout << "generate permutations:"
    //              << std::chrono::duration_cast<std::chrono::milliseconds>(t2
    //              - t1)
    //                     .count()
    //              << '\n';

    //    auto t3 = std::chrono::high_resolution_clock::now();

    std::vector<Slice> slices = makeSlices(permutations, rows, cluePairs);

    //    auto t4 = std::chrono::high_resolution_clock::now();
    //    std::cout << "make slices:"
    //              << std::chrono::duration_cast<std::chrono::milliseconds>(t4
    //              - t3)
    //                     .count()
    //              << '\n';

    //    auto t5 = std::chrono::high_resolution_clock::now();

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

    //    auto t6 = std::chrono::high_resolution_clock::now();
    //    std::cout << "solving loop:"
    //              << std::chrono::duration_cast<std::chrono::milliseconds>(t6
    //              - t5)
    //                     .count()
    //              << '\n';

    return board.skyscrapers;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace permutation
