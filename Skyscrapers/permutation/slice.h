#ifndef PERMUTATION_SLICE_H
#define PERMUTATION_SLICE_H

#include "span.h"

#include <set>
#include <vector>

namespace permutation {

class FieldElements;
class Permutations;
class Row;
class Field;
class CluePair;

class Slice {
public:
    Slice(Permutations &permutations,
          const std::vector<std::size_t> &permutationIndexes, Row &row);

    void guessSkyscraperOutOfNeighbourNopes();

    bool isSolved() const;

    void solveFromPossiblePermutations();

    bool reducePossiblePermutations();

private:
    std::vector<std::set<int>> getPossibleBuildings() const;

    FieldElements
    getFieldElements(const std::vector<std::set<int>> &possibleBuildings);

    Permutations *mPermutations;
    std::vector<std::size_t> mPermutationIndexes;
    Row *mRow;
};

std::vector<Slice> makeSlices(Permutations &permutations,
                              std::vector<Row> &rows,
                              const std::vector<CluePair> &cluePairs);

std::vector<Field> copyField(const std::vector<Field *> &currFields);

bool fieldsIdentical(const std::vector<Field> &lastFields,
                     const std::vector<Field *> &currFields);

bool isValidPermutation(const Span<int> &permutation,
                        const std::vector<Field *> fields);

} // namespace permutation
#endif
