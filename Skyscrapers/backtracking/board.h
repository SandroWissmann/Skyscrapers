#ifndef BACKTRACKING_BOARD_H
#define BACKTRACKING_BOARD_H

#include "field.h"
#include "nopes.h"
#include "row.h"

#include <string>
#include <vector>

namespace backtracking {
struct Board {
    Board(std::size_t size);

    std::vector<std::vector<int>> skyscrapers{};
    std::vector<std::vector<Nopes>> nopes;

private:
    std::vector<std::vector<int>> makeSkyscrapers(std::size_t size);
    std::vector<std::vector<Nopes>> makeNopes(std::size_t size);

    std::vector<std::vector<Field>> mFields;
    std::vector<Row> mRows;

    void makeFields();
    void makeRows();
    void connnectRowsWithCrossingRows();
};

void debug_print(Board &board, const std::string &title = "");

} // namespace backtracking

#endif
