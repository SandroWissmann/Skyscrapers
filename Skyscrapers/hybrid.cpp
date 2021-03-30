#include "hybrid.h"

#include "backtracking.h"
#include "permutation.h"

#include "shared/board.h"
#include "shared/rowclues.h"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace hybrid {

double cluesFactor(const std::vector<int> &clues)
{
    int clueCount = 0;
    for (const auto &clue : clues) {
        if (clue != 0) {
            ++clueCount;
        }
    }
    return static_cast<double>(clueCount) / clues.size();
}

double startingGridFactor(const std::vector<std::vector<int>> &startingGrid)
{
    std::size_t fieldCount = startingGrid.size() * startingGrid[0].size();

    int skyscraperCount = 0;
    for (const auto &row : startingGrid) {
        for (const auto &skyscraper : row) {
            if (skyscraper != 0) {
                ++skyscraperCount;
            }
        }
    }
    return static_cast<double>(skyscraperCount) / fieldCount;
}

double skyscraperFactor(const Board &board)
{
    int skyscraperCount = 0;
    for (const auto &field : board.fields) {
        if (field.hasSkyscraper()) {
            ++skyscraperCount;
        }
    }
    return static_cast<double>(skyscraperCount) / board.fields.size();
}

double nopeFactor(const Board &board)
{
    double nopeCount = 0;
    auto boardSize = board.size();
    for (const auto &field : board.fields) {
        if (!field.nopes(board.size()).empty()) {
            auto nopes = field.nopes(board.size());
            nopeCount += static_cast<double>(nopes.size()) / boardSize;
        }
    }
    return static_cast<double>(nopeCount) / board.fields.size();
}

double fieldFactor(const Board &board)
{
    double fieldCount = 0;
    auto boardSize = board.size();
    for (const auto &field : board.fields) {
        if (field.hasSkyscraper()) {
            ++fieldCount;
        }
        else {
            auto nopes = field.nopes(board.size());
            fieldCount += static_cast<double>(nopes.size()) / boardSize;
        }
    }
    return fieldCount / board.fields.size();
}

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

    //    debug_print(board);

    //    std::cout << "cluesFactor:" << cluesFactor(clues) << '\n';
    //    std::cout << "startingGridFactor" << startingGridFactor(startingGrid)
    //              << '\n';
    //    std::cout << "skyscraperFactor:" << skyscraperFactor(board) << '\n';
    //    std::cout << "nopeFactor:" << nopeFactor(board) << '\n';
    std::cout << "fieldFactor:" << fieldFactor(board) << '\n';

    if (board.size() >= 11) {
        if (fieldFactor(board) < 0.8) {
            std::cout << "PERMUTATION\n";
            return permutation::SolvePuzzle(clues, startingGrid, 0);
        }
        std::cout << "PERMUTATION\n";
        return backtracking::SolvePuzzle(clues, startingGrid, 0);
    }
    if (fieldFactor(board) < 0.7) {
        std::cout << "PERMUTATION\n";
        return permutation::SolvePuzzle(clues, startingGrid, 0);
    }
    std::cout << "BACKTRACKING\n";
    return backtracking::SolvePuzzle(clues, startingGrid, 0);
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    std::cout << "PERMUTATION:\n";
    return permutation::SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace hybrid
