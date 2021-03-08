#ifndef PERMUTATION_FIELDELEMENTS_H
#define PERMUTATION_FIELDELEMENTS_H

#include "nopes.h"

#include <vector>

namespace permutation {
struct FieldElements {
    std::vector<int> skyscrapers{};
    std::vector<std::vector<int>> nopes{};
};
} // namespace permutation

#endif
