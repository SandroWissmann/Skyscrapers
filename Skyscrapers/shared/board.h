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

    std::vector<int> skyscrapers{};
    std::vector<Nopes> nopes;

    std::vector<Row> mRows;

    std::vector<std::vector<int>> skyscrapers2d() const;

    std::size_t size() const;

private:
    void makeFields();
    void makeRows();
    void connnectRowsWithCrossingRows();

    std::size_t mSize;
    std::vector<Field> mFields;
};

void debug_print(Board &board, const std::string &title = "");

#endif
