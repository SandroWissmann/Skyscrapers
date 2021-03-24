#include "row.h"

#include "borderiterator.h"
#include "field.h"
#include "missingnumberinsequence.h"
#include "point.h"

#include <algorithm>
#include <cassert>
#include <unordered_map>

Row::Row(std::vector<Field> &fields, std::size_t size, const Point &startPoint,
         const ReadDirection &readDirection)
    : mRowFields{getRowFields(readDirection, fields, size, startPoint)}
{
}

std::size_t Row::size() const
{
    return mRowFields.size();
}

void Row::addCrossingRows(Row *crossingRow)
{
    assert(crossingRow != nullptr);
    assert(mCrossingRows.size() < size());
    mCrossingRows.push_back(crossingRow);
}

bool Row::hasOnlyOneNopeField() const
{
    return skyscraperCount() == static_cast<int>(size() - 1);
}

void Row::addLastMissingSkyscraper()
{
    assert(hasOnlyOneNopeField());

    auto nopeFieldIt = mRowFields.end();
    std::vector<int> sequence;
    sequence.reserve(size() - 1);

    for (auto it = mRowFields.begin(); it != mRowFields.end(); ++it) {
        if ((*it)->hasSkyscraper()) {
            sequence.emplace_back((*it)->skyscraper(size()));
        }
        else {
            nopeFieldIt = it;
        }
    }
    assert(nopeFieldIt != mRowFields.end());
    assert(skyscraperCount() == static_cast<int>(sequence.size()));

    auto missingValue =
        missingNumberInSequence(sequence.begin(), sequence.end());

    assert(missingValue >= 0 && missingValue <= static_cast<int>(size()));

    (*nopeFieldIt)->insertSkyscraper(missingValue);
    insertSkyscraperNeighbourHandling(nopeFieldIt, missingValue);
}

void Row::addNopesToAllNopeFields(int nope)
{
    for (auto it = mRowFields.begin(); it != mRowFields.end(); ++it) {
        if ((*it)->hasSkyscraper()) {
            continue;
        }

        bool hasSkyscraperBefore = false;
        (*it)->insertNope(nope);
        insertNopesNeighbourHandling(it, nope, hasSkyscraperBefore);
    }
}

bool Row::allFieldsContainSkyscraper() const
{
    return skyscraperCount() == static_cast<int>(size());
}

int Row::skyscraperCount() const
{
    int count = 0;
    for (auto cit = mRowFields.cbegin(); cit != mRowFields.cend(); ++cit) {
        if ((*cit)->hasSkyscraper()) {
            ++count;
        }
    }
    return count;
}

int Row::nopeCount(int nope) const
{
    int count = 0;
    for (auto cit = mRowFields.cbegin(); cit != mRowFields.cend(); ++cit) {
        if ((*cit)->hasSkyscraper()) {
            continue;
        }
        if ((*cit)->containsNope(nope)) {
            ++count;
        }
    }
    return count;
}

void Row::guessSkyscraperOutOfNeighbourNopes()
{
    for (;;) {
        auto optNope = nopeValueInAllButOneField();
        if (!optNope) {
            break;
        }
        insertSkyscraperToFirstFieldWithoutNope(*optNope);
    }
}

bool Row::hasSkyscrapers(const std::vector<int> &skyscrapers,
                         Row::Direction direction) const
{
    if (direction == Direction::front) {
        return hasSkyscrapers(skyscrapers.cbegin(), skyscrapers.cend(),
                              mRowFields.cbegin(), mRowFields.cend());
    }
    return hasSkyscrapers(skyscrapers.cbegin(), skyscrapers.cend(),
                          mRowFields.crbegin(), mRowFields.crend());
}

bool Row::hasNopes(const std::vector<std::vector<int>> &nopes,
                   Direction direction) const
{
    if (direction == Direction::front) {
        return hasNopes(nopes.cbegin(), nopes.cend(), mRowFields.cbegin(),
                        mRowFields.cend());
    }
    return hasNopes(nopes.cbegin(), nopes.cend(), mRowFields.crbegin(),
                    mRowFields.crend());
}

void Row::addFieldData(const std::vector<Field> &fieldData, Direction direction)
{
    if (direction == Direction::front) {
        addFieldData(fieldData.begin(), fieldData.end(), mRowFields.begin(),
                     mRowFields.end());
    }
    else {
        addFieldData(fieldData.begin(), fieldData.end(), mRowFields.rbegin(),
                     mRowFields.rend());
    }
}

std::vector<Field *> Row::getFields() const
{
    return mRowFields;
}

template <typename SkyIterator, typename FieldIterator>
bool Row::hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                         FieldIterator fieldItBegin,
                         FieldIterator fieldItEnd) const
{
    auto skyIt = skyItBegin;
    for (auto fieldIt = fieldItBegin;
         fieldIt != fieldItEnd && skyIt != skyItEnd; ++fieldIt, ++skyIt) {
        if (*skyIt == 0 && (*fieldIt)->hasSkyscraper()) {
            continue;
        }
        if ((*fieldIt)->skyscraper(size()) != *skyIt) {
            return false;
        }
    }
    return true;
}

template <typename NopesIterator, typename FieldIterator>
bool Row::hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                   FieldIterator fieldItBegin, FieldIterator fieldItEnd) const
{
    auto nopesIt = nopesItBegin;
    for (auto fieldIt = fieldItBegin;
         fieldIt != fieldItEnd && nopesIt != nopesItEnd; ++fieldIt, ++nopesIt) {

        if (nopesIt->empty()) {
            continue;
        }
        if ((*fieldIt)->hasSkyscraper()) {
            return false;
        }
        if (!(*fieldIt)->containsNopes(*nopesIt)) {
            return false;
        }
    }
    return true;
}

template <typename FieldDataIterator, typename FieldIterator>
void Row::addFieldData(FieldDataIterator fieldDataItBegin,
                       FieldDataIterator fieldDataItEnd,
                       FieldIterator fieldItBegin, FieldIterator fieldItEnd)
{
    auto fieldDataIt = fieldDataItBegin;
    for (auto fieldIt = fieldItBegin;
         fieldIt != fieldItEnd && fieldDataIt != fieldDataItEnd;
         ++fieldIt, ++fieldDataIt) {
        insertFieldData(fieldIt, *fieldDataIt);
    }
}

template <typename FieldIterator>
void Row::insertFieldData(const FieldIterator &fieldIt, const Field &fieldData)
{
    if (fieldData.hasSkyscraper()) {
        if ((*fieldIt)->hasSkyscraper()) {
            return;
        }
        (*fieldIt)->setBitmask(fieldData.bitmask());
        insertSkyscraperNeighbourHandling(fieldIt,
                                          (*fieldIt)->skyscraper(size()));
    }
    else {
        bool hasSkyscraperBefore = (*fieldIt)->hasSkyscraper();
        (*fieldIt)->insertNopes(fieldData);

        auto nopes = fieldData.nopes(size());

        for (const auto &nope : nopes) {
            insertNopesNeighbourHandling(fieldIt, nope, hasSkyscraperBefore);
        }
    }
}

template <typename FieldIterator>
void Row::insertSkyscraperNeighbourHandling(FieldIterator fieldIt,
                                            int skyscraper)
{
    if (hasOnlyOneNopeField()) {
        addLastMissingSkyscraper();
    }
    addNopesToAllNopeFields(skyscraper);

    int idx = getIdx(fieldIt);

    if (mCrossingRows[idx]->hasOnlyOneNopeField()) {
        mCrossingRows[idx]->addLastMissingSkyscraper();
    }

    mCrossingRows[idx]->addNopesToAllNopeFields(skyscraper);
}

template <typename FieldIterator>
void Row::insertNopesNeighbourHandling(FieldIterator fieldIt, int nope,
                                       bool hadSkyscraperBefore)
{
    // skyscraper was added so we have to add nopes to the neighbours
    if (!hadSkyscraperBefore && (*fieldIt)->hasSkyscraper()) {

        insertSkyscraperNeighbourHandling(fieldIt,
                                          (*fieldIt)->skyscraper(size()));
    }

    if (onlyOneFieldWithoutNope(nope)) {
        insertSkyscraperToFirstFieldWithoutNope(nope);
    }

    int idx = getIdx(fieldIt);

    if (mCrossingRows[idx]->onlyOneFieldWithoutNope(nope)) {
        mCrossingRows[idx]->insertSkyscraperToFirstFieldWithoutNope(nope);
    }
}

int Row::getIdx(std::vector<Field *>::const_iterator cit) const
{
    return std::distance(mRowFields.cbegin(), cit);
}

int Row::getIdx(std::vector<Field *>::const_reverse_iterator crit) const
{
    return size() - std::distance(mRowFields.crbegin(), crit) - 1;
}

std::vector<Field *> Row::getRowFields(const ReadDirection &readDirection,
                                       std::vector<Field> &boardFields,
                                       std::size_t size,
                                       const Point &startPoint)
{
    std::vector<Field *> fields;
    fields.reserve(size);
    std::size_t x = startPoint.x;
    std::size_t y = startPoint.y;

    if (readDirection == ReadDirection::topToBottom) {
        for (std::size_t i = 0; i < size; ++i) {
            fields.emplace_back(&boardFields[x + y * size]);
            ++y;
        }
    }
    else { // ReadDirection::rightToLeft
        for (std::size_t i = 0; i < size; ++i) {
            fields.emplace_back(&boardFields[x + y * size]);
            --x;
        }
    }
    return fields;
}

bool Row::onlyOneFieldWithoutNope(int nope) const
{
    if (nopeExistsAsSkyscraperInFields(mRowFields, nope)) {
        return false;
    }
    if (nopeCount(nope) < static_cast<int>(size()) - skyscraperCount() - 1) {
        return false;
    }
    return true;
}

bool Row::nopeExistsAsSkyscraperInFields(const std::vector<Field *> &rowFields,
                                         int nope) const
{
    auto cit = std::find_if(rowFields.cbegin(), rowFields.cend(),
                            [nope, size = size()](const auto &field) {
                                return field->skyscraper(size) == nope;
                            });
    return cit != rowFields.cend();
}

std::optional<int> Row::nopeValueInAllButOneField() const
{
    std::unordered_map<int, int> nopeAndCount;

    for (auto cit = mRowFields.cbegin(); cit != mRowFields.cend(); ++cit) {
        if (!(*cit)->hasSkyscraper()) {
            auto nopes = (*cit)->nopes(size());
            for (const auto &nope : nopes) {
                if (hasSkyscraper(nope)) {
                    continue;
                }
                ++nopeAndCount[nope];
            }
        }
    }
    for (auto cit = nopeAndCount.cbegin(); cit != nopeAndCount.end(); ++cit) {
        if (cit->second == static_cast<int>(size()) - skyscraperCount() - 1) {
            return {cit->first};
        }
    }
    return {};
}

void Row::insertSkyscraperToFirstFieldWithoutNope(int nope)
{
    for (auto it = mRowFields.begin(); it != mRowFields.end(); ++it) {
        if ((*it)->hasSkyscraper()) {
            continue;
        }
        if (!(*it)->containsNope(nope)) {
            (*it)->insertSkyscraper(nope);
            insertSkyscraperNeighbourHandling(it, nope);
            return; // there can be max one skyscraper per row;
        }
    }
}

bool Row::hasSkyscraper(int skyscraper) const
{
    for (const auto &field : mRowFields) {
        if (field->skyscraper(size()) == skyscraper) {
            return true;
        }
    }
    return false;
}
