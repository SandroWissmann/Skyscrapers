#include "slice.h"

#include "../shared/field.h"
#include "../shared/row.h"
#include "permutations.h"

namespace permutation {

Slice::Slice(Permutations &permutations,
             const std::vector<std::size_t> &permutationIndexes, Row &row,
             std::size_t size)
    : mPermutations{&permutations},
      mPermutationIndexes{permutationIndexes}, mRow{&row}
{
    if (permutationIndexes.empty()) {
        return;
    }

    auto possibleBuildings = getPossibleBuildings(size);
    auto fieldElements = getFieldElements(possibleBuildings);

    mRow->addFieldData(fieldElements, Row::Direction::front);
}

void Slice::guessSkyscraperOutOfNeighbourNopes()
{
    mRow->guessSkyscraperOutOfNeighbourNopes();
}

bool Slice::isSolved() const
{
    return mRow->allFieldsContainSkyscraper();
}

void Slice::solveFromPossiblePermutations(std::size_t size)
{
    if (mPermutationIndexes.empty()) {
        return;
    }

    while (reducePossiblePermutations(size)) {
        auto lastFields = copyFields(size);

        auto possibleBuildings = getPossibleBuildings(size);
        auto fieldElements = getFieldElements(possibleBuildings);

        mRow->addFieldData(fieldElements, Row::Direction::front);

        if (fieldsIdentical(lastFields, size)) {
            break;
        }
    }
}

bool Slice::reducePossiblePermutations(std::size_t size)
{
    auto startSize = mPermutationIndexes.size();
    auto it = mPermutationIndexes.begin();

    while (it != mPermutationIndexes.end()) {

        if (!isValidPermutation(mPermutations->operator[](*it), size)) {
            it = mPermutationIndexes.erase(it);
        }
        else {
            ++it;
        }
    }

    return startSize > mPermutationIndexes.size();
}

std::vector<std::set<int>> Slice::getPossibleBuildings(std::size_t size) const
{
    std::vector<std::set<int>> possibleBuildingsOnFields(size);

    for (const auto &permutationIndex : mPermutationIndexes) {

        auto permutation = mPermutations->operator[](permutationIndex);
        for (std::size_t i = 0; i < permutation.size(); ++i) {
            possibleBuildingsOnFields[i].insert(permutation[i]);
        }
    }
    return possibleBuildingsOnFields;
}

std::vector<Field> Slice::copyFields(std::size_t size) const
{
    std::vector<Field> result;
    result.reserve(size);

    for (std::size_t idx = 0; idx < size; ++idx) {
        Field field;
        auto bitmask = mRow->getFieldRef(idx).bitmask();
        field.setBitmask(bitmask);
        result.emplace_back(field);
    }
    return result;
}

bool Slice::fieldsIdentical(const std::vector<Field> &lastFields,
                            std::size_t size) const
{
    assert(lastFields.size == size);

    for (std::size_t idx = 0; idx < size; ++idx) {
        if (lastFields[idx].bitmask() != mRow->getFieldRef(idx).bitmask()) {
            return false;
        }
    }
    return true;
}

bool Slice::isValidPermutation(const Span<int> &permutation,
                               std::size_t size) const
{
    assert(permutation.size() == size);

    for (std::size_t idx = 0; idx < size; ++idx) {
        if (mRow->getFieldRef(idx).hasSkyscraper()) {
            if (mRow->getFieldRef(idx).skyscraper(size) != permutation[idx]) {
                return false;
            }
        }
        else if (!mRow->getFieldRef(idx).nopes(size).empty()) {
            if (mRow->getFieldRef(idx).containsNope(permutation[idx])) {
                return false;
            }
        }
    }
    return true;
}

std::vector<Field>
Slice::getFieldElements(const std::vector<std::set<int>> &possibleBuildings)
{
    std::vector<Field> fieldElements(possibleBuildings.size(), Field{});

    for (std::size_t i = 0; i < possibleBuildings.size(); ++i) {
        if (possibleBuildings[i].size() == 1) {
            fieldElements[i].insertSkyscraper(*possibleBuildings[i].begin());
        }
        else {
            std::vector<int> nopes;
            nopes.reserve(possibleBuildings.size());
            for (std::size_t j = 0; j < possibleBuildings.size(); ++j) {
                auto it = possibleBuildings[i].find(j + 1);

                if (it == possibleBuildings[i].end()) {
                    nopes.emplace_back(j + 1);
                }
            }
            fieldElements[i].insertNopes(nopes);
        }
    }
    return fieldElements;
}

std::vector<Slice> makeSlices(Permutations &permutations,
                              std::vector<Row> &rows,
                              const std::vector<CluePair> &cluePairs,
                              std::size_t size)
{
    std::vector<Slice> slices;
    slices.reserve(rows.size());

    for (std::size_t i = 0; i < cluePairs.size(); ++i) {
        slices.emplace_back(Slice{
            permutations, permutations.permutationIndexs(i), rows[i], size});
    }

    return slices;
}

} // namespace permutation
