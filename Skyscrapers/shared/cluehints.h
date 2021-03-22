#ifndef CLUEHINTS_H
#define CLUEHINTS_H

#include "field.h"

#include <cstddef>
#include <vector>

struct ClueHints {
    ClueHints(std::size_t boardSize);
    ClueHints() = default;

    void reverse();

    bool isEmpty() const;

    std::vector<Field> fields;
};

std::vector<ClueHints> getClueHints(const std::vector<int> &clues,
                                    std::size_t boardSize);

ClueHints getClueHints(int clue, std::size_t boardSize);

void mergeClueHintsPerRow(std::vector<ClueHints> &clueHints);

ClueHints merge(ClueHints frontClueHints, ClueHints backClueHints);

#endif
