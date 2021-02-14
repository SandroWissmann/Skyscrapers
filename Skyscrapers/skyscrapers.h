#ifndef SKYSCRAPER_H
#define SKYSCRAPER_H

#include <vector>

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues);

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> starting_grid, int N);

#endif
