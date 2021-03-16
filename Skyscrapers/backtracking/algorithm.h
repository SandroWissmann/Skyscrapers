#ifndef BACKTRACKING_ALGORITHM_H
#define BACKTRACKING_ALGORITHM_H

#include <vector>

class Board;

namespace backtracking {

bool guessSkyscrapers(Board &board, const std::vector<int> &clues,
                      std::size_t index, std::size_t countOfElements,
                      std::size_t rowSize);

bool skyscrapersAreValidPositioned(const std::vector<int> &skyscrapers,
                                   const std::vector<int> &clues,
                                   std::size_t index, std::size_t rowSize);

bool rowsAreValid(const std::vector<int> &skyscrapers, std::size_t index,
                  std::size_t rowSize);

bool columnsAreValid(const std::vector<int> &skyscrapers, std::size_t index,
                     std::size_t rowSize);

bool rowCluesAreValid(const std::vector<int> &skyscrapers,
                      const std::vector<int> &clues, std::size_t index,
                      std::size_t rowSize);

std::tuple<int, int> getRowClues(const std::vector<int> &clues, std::size_t row,
                                 std::size_t rowSize);

bool columnCluesAreValid(const std::vector<int> &skyscrapers,
                         const std::vector<int> &clues, std::size_t index,
                         std::size_t rowSize);

std::tuple<int, int> getColumnClues(const std::vector<int> &clues,
                                    std::size_t column, std::size_t rowSize);

template <typename Iterator> int visibleBuildings(Iterator begin, Iterator end)
{
    int visibleBuildingsCount = 0;
    int highestSeen = 0;
    for (auto it = begin; it != end; ++it) {
        if (*it != 0 && *it > highestSeen) {
            ++visibleBuildingsCount;
            highestSeen = *it;
        }
    }
    return visibleBuildingsCount;
}

} // namespace backtracking
#endif
