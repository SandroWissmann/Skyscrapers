#ifndef BOARD_H
#define BOARD_H

#include "field.h"
#include "nopes.h"
#include "row.h"

#include <string>
#include <vector>

class ClueHints;

struct Board {
    Board(std::size_t size);

    void insert(const std::vector<std::optional<ClueHints>> &clueHints);

    void insert(const std::vector<std::vector<int>> &startingSkyscrapers);

    bool isSolved() const;

    std::vector<std::vector<int>> skyscrapers{};
    std::vector<std::vector<Nopes>> nopes;

    std::vector<Row> mRows;

private:
    std::vector<std::vector<int>> makeSkyscrapers(std::size_t size);
    std::vector<std::vector<Nopes>> makeNopes(std::size_t size);

    void makeFields();
    void makeRows();
    void connnectRowsWithCrossingRows();

    std::vector<std::vector<Field>> mFields;
};

void debug_print(Board &board, const std::string &title = "");

#endif
