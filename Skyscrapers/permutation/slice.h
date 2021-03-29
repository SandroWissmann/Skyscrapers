#ifndef PERMUTATION_SLICE_H
#define PERMUTATION_SLICE_H

#include "span.h"

#include <set>
#include <vector>

class Field;
class Row;

namespace permutation {

class FieldElements;
class Permutations;
class CluePair;

class Slice {
public:
    Slice(Permutations &permutations,
          const std::vector<std::size_t> &permutationIndexes, Row &row,
          std::size_t size);

    void guessSkyscraperOutOfNeighbourNopes();

    bool isSolved() const;

    void solveFromPossiblePermutations(std::size_t size);

    bool reducePossiblePermutations(std::size_t size);

private:
    std::vector<std::set<int>> getPossibleBuildings(std::size_t size) const;

    std::vector<Field> copyFields(std::size_t size) const;

    bool fieldsIdentical(const std::vector<Field> &lastFields,
                         std::size_t size) const;

    bool isValidPermutation(const Span<int> &permutation,
                            std::size_t size) const;

    std::vector<Field>
    getFieldElements(const std::vector<std::set<int>> &possibleBuildings);

    Permutations *mPermutations;
    std::vector<std::size_t> mPermutationIndexes;
    Row *mRow;
};

std::vector<Slice> makeSlices(Permutations &permutations,
                              std::vector<Row> &rows,
                              const std::vector<CluePair> &cluePairs,
                              std::size_t size);

} // namespace permutation
#endif
