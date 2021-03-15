#ifndef BACKTRACKING_ALGORITHM_H
#define BACKTRACKING_ALGORITHM_H

#include <vector>

class Board;

namespace backtracking {

bool guessSkyscrapers(Board &board, const std::vector<int> &clues,
                      std::size_t x, std::size_t y, std::size_t size);

bool skyscrapersAreValidPositioned(
    const std::vector<std::vector<int>> &skyscrapers,
    const std::vector<int> &clues, std::size_t x, std::size_t y,
    std::size_t size);

bool rowsAreValid(const std::vector<std::vector<int>> &skyscrapers,
                  std::size_t x, std::size_t y, std::size_t size);

bool columnsAreValid(const std::vector<std::vector<int>> &skyscrapers,
                     std::size_t x, std::size_t y, std::size_t size);

bool rowCluesAreValid(const std::vector<std::vector<int>> &skyscrapers,
                      const std::vector<int> &clues, std::size_t y,
                      std::size_t size);

std::tuple<int, int> getRowClues(const std::vector<int> &clues, std::size_t y,
                                 std::size_t size);

bool columnCluesAreValid(const std::vector<std::vector<int>> &skyscrapers,
                         const std::vector<int> &clues, std::size_t x,
                         std::size_t size);

std::tuple<int, int> getColumnClues(const std::vector<int> &clues,
                                    std::size_t x, std::size_t size);

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
