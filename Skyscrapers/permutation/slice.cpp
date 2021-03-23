#include "slice.h"

#include "../shared/field.h"
#include "../shared/row.h"
#include "fieldelements.h"
#include "permutations.h"

namespace permutation {

Slice::Slice(Permutations &permutations,
             const std::vector<std::size_t> &permutationIndexes, Row &row)
    : mPermutations{&permutations},
      mPermutationIndexes{permutationIndexes}, mRow{&row}
{
    if (permutationIndexes.empty()) {
        return;
    }

    auto possibleBuildings = getPossibleBuildings();
    auto fieldElements = getFieldElements(possibleBuildings);

    mRow->addSkyscrapers(fieldElements.skyscrapers, Row::Direction::front);
    mRow->addNopes(fieldElements.nopes, Row::Direction::front);
}

void Slice::guessSkyscraperOutOfNeighbourNopes()
{
    mRow->guessSkyscraperOutOfNeighbourNopes();
}

bool Slice::isSolved() const
{
    return mRow->allFieldsContainSkyscraper();
}

void Slice::solveFromPossiblePermutations()
{
    if (mPermutationIndexes.empty()) {
        return;
    }

    while (reducePossiblePermutations()) {

        auto lastFields = copyField(mRow->getFields());

        auto possibleBuildings = getPossibleBuildings();
        auto fieldElements = getFieldElements(possibleBuildings);

        mRow->addSkyscrapers(fieldElements.skyscrapers, Row::Direction::front);
        mRow->addNopes(fieldElements.nopes, Row::Direction::front);

        if (fieldsIdentical(lastFields, mRow->getFields())) {
            break;
        }
    }
}

bool Slice::reducePossiblePermutations()
{
    auto startSize = mPermutationIndexes.size();
    auto it = mPermutationIndexes.begin();

    auto fields = mRow->getFields();

    while (it != mPermutationIndexes.end()) {

        if (!isValidPermutation(mPermutations->operator[](*it), fields)) {
            it = mPermutationIndexes.erase(it);
        }
        else {
            ++it;
        }
    }

    return startSize > mPermutationIndexes.size();
}

std::vector<std::set<int>> Slice::getPossibleBuildings() const
{
    std::vector<std::set<int>> possibleBuildingsOnFields(mRow->size());

    for (const auto &permutationIndex : mPermutationIndexes) {

        auto permutation = mPermutations->operator[](permutationIndex);
        for (std::size_t i = 0; i < permutation.size(); ++i) {
            possibleBuildingsOnFields[i].insert(permutation[i]);
        }
    }
    return possibleBuildingsOnFields;
}

FieldElements
Slice::getFieldElements(const std::vector<std::set<int>> &possibleBuildings)
{
    FieldElements fieldElements;
    fieldElements.skyscrapers.reserve(possibleBuildings.size());
    fieldElements.nopes.reserve(possibleBuildings.size());

    for (std::size_t i = 0; i < possibleBuildings.size(); ++i) {
        if (possibleBuildings[i].size() == 1) {
            fieldElements.skyscrapers.emplace_back(
                *possibleBuildings[i].begin());
            fieldElements.nopes.emplace_back(std::vector<int>{});
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
            fieldElements.skyscrapers.emplace_back(0);
            fieldElements.nopes.emplace_back(nopes);
        }
    }
    return fieldElements;
}

std::vector<Slice> makeSlices(Permutations &permutations,
                              std::vector<Row> &rows,
                              const std::vector<CluePair> &cluePairs)
{
    std::vector<Slice> slices;
    slices.reserve(rows.size());

    for (std::size_t i = 0; i < cluePairs.size(); ++i) {
        slices.emplace_back(
            Slice{permutations, permutations.permutationIndexs(i), rows[i]});
    }

    return slices;
}

std::vector<Field> copyField(const std::vector<Field *> &currFields)
{
    std::vector<Field> result;
    result.reserve(currFields.size());

    for (const auto &currField : currFields) {
        result.emplace_back(Field{*currField});
    }
    return result;
}

bool fieldsIdentical(const std::vector<Field> &lastFields,
                     const std::vector<Field *> &currFields)
{
    if (lastFields.size() != currFields.size()) {
        return false;
    }

    auto lastIt = lastFields.begin();
    auto currPtrIt = currFields.begin();

    for (; lastIt != lastFields.end(); ++lastIt, ++currPtrIt) {
        if (*lastIt != **currPtrIt) {
            return false;
        }
    }
    return true;
}

bool isValidPermutation(const Span<int> &permutation,
                        const std::vector<Field *> fields)
{
    auto permIt = permutation.cbegin();
    for (auto fieldIt = fields.cbegin();
         fieldIt != fields.cend() && permIt != permutation.cend();
         ++fieldIt, ++permIt) {

        if ((*fieldIt)->hasSkyscraper()) {
            if ((*fieldIt)->skyscraper(fields.size()) != *permIt) {
                return false;
            }
        }
        else if (!(*fieldIt)->nopes(fields.size()).empty()) {
            if ((*fieldIt)->containsNope(*permIt)) {
                return false;
            }
        }
    }
    return true;
}

} // namespace permutation
