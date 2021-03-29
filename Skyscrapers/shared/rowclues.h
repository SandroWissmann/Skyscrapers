#ifndef ROW_CLUES_H
#define ROW_CLUES_H

#include "field.h"

#include <cstddef>
#include <vector>

struct RowClues {
    RowClues(std::size_t boardSize);
    RowClues() = default;

    void reverse();

    bool isEmpty() const;

    std::vector<Field> fields;
};

std::vector<RowClues> getRowClues(const std::vector<int> &clues,
                                  std::size_t boardSize);

RowClues getRowClue(int clue, std::size_t boardSize);

void mergeClueHintsPerRow(std::vector<RowClues> &rowClues);

RowClues merge(RowClues frontRowClues, RowClues backRowClues);

#endif
