#include "row.h"

#include "board.h"
#include "borderiterator.h"
#include "field.h"
#include "missingnumberinsequence.h"
#include "point.h"

#include <algorithm>
#include <cassert>
#include <unordered_map>

Row::Row(Board &board, const Point &startPoint,
         const ReadDirection &readDirection)
    : mBoard{board}, mStartPoint{startPoint}, mReadDirection{readDirection}
{
}

void Row::addCrossingRows(Row *crossingRow)
{
    assert(crossingRow != nullptr);
    assert(mCrossingRows.size() < mBoard.size());
    mCrossingRows.push_back(crossingRow);
}

bool Row::hasOnlyOneNopeField() const
{
    return skyscraperCount() == static_cast<int>(mBoard.size() - 1);
}

void Row::addLastMissingSkyscraper()
{
    assert(hasOnlyOneNopeField());

    std::vector<int> sequence;
    sequence.reserve(mBoard.size() - 1);

    std::size_t nopeFieldIdx = -1;
    for (std::size_t idx = 0; idx < mBoard.size(); ++idx) {
        if (getFieldRef(idx).hasSkyscraper()) {
            sequence.emplace_back((getFieldRef(idx)).skyscraper(mBoard.size()));
        }
        else {
            nopeFieldIdx = idx;
        }
    }

    assert(nopeFieldIdx != -1);
    assert(skyscraperCount() == static_cast<int>(sequence.size()));

    auto missingValue =
        missingNumberInSequence(sequence.begin(), sequence.end());

    assert(missingValue >= 0 && missingValue <= static_cast<int>(size()));

    (getFieldRef(nopeFieldIdx)).insertSkyscraper(missingValue);
    insertSkyscraperNeighbourHandling(nopeFieldIdx, missingValue);
}

void Row::addNopesToAllNopeFields(int nope)
{
    for (std::size_t idx = 0; idx < mBoard.size(); ++idx) {
        if (getFieldRef(idx).hasSkyscraper()) {
            continue;
        }

        bool hasSkyscraperBefore = false;
        getFieldRef(idx).insertNope(nope);
        insertNopesNeighbourHandling(idx, nope, hasSkyscraperBefore);
    }
}

bool Row::allFieldsContainSkyscraper() const
{
    return skyscraperCount() == static_cast<int>(mBoard.size());
}

int Row::skyscraperCount() const
{
    int count = 0;

    for (std::size_t i = 0; i < mBoard.size(); ++i) {
        if (getFieldRef(i).hasSkyscraper()) {
            ++count;
        }
    }
    return count;
}

int Row::nopeCount(int nope) const
{
    int count = 0;
    for (std::size_t i = 0; i < mBoard.size(); ++i) {
        if (getFieldRef(i).hasSkyscraper()) {
            continue;
        }
        if (getFieldRef(i).containsNope(nope)) {
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
        return hasSkyscrapers(skyscrapers.cbegin(), skyscrapers.cend());
    }
    return hasSkyscrapers(skyscrapers.crbegin(), skyscrapers.crend());
}

bool Row::hasNopes(const std::vector<std::vector<int>> &nopes,
                   Direction direction) const
{
    if (direction == Direction::front) {
        return hasNopes(nopes.cbegin(), nopes.cend());
    }
    return hasNopes(nopes.crbegin(), nopes.crend());
}

void Row::addFieldData(const std::vector<Field> &fieldData, Direction direction)
{
    if (direction == Direction::front) {
        addFieldData(fieldData.begin(), fieldData.end());
    }
    else {
        addFieldData(fieldData.rbegin(), fieldData.rend());
    }
}

template <typename SkyIterator>
bool Row::hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd) const
{
    for (auto skyIt = skyItBegin; skyIt != skyItEnd; ++skyIt) {
        auto idx = std::distance(skyItBegin, skyIt);
        if (*skyIt == 0 && getFieldRef(idx).hasSkyscraper()) {
            continue;
        }
        if (getFieldRef(idx).skyscraper(mBoard.size()) != *skyIt) {
            return false;
        }
    }
    return true;
}

template <typename NopesIterator>
bool Row::hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd) const
{
    for (auto nopesIt = nopesItBegin; nopesIt != nopesItEnd; ++nopesIt) {
        if (nopesIt->empty()) {
            continue;
        }
        auto idx = std::distance(nopesItBegin, nopesIt);
        if (getFieldRef(idx).hasSkyscraper()) {
            return false;
        }
        if (!getFieldRef(idx).containsNopes(*nopesIt)) {
            return false;
        }
    }
    return true;
}

template <typename FieldDataIterator>
void Row::addFieldData(FieldDataIterator fieldDataItBegin,
                       FieldDataIterator fieldDataItEnd)
{
    for (auto fieldDataIt = fieldDataItBegin; fieldDataIt != fieldDataItEnd;
         ++fieldDataIt) {

        auto idx = std::distance(fieldDataItBegin, fieldDataIt);
        insertFieldData(idx, *fieldDataIt);
    }
}

const Field &Row::getFieldRef(std::size_t idx) const
{
    assert(idx >= 0 && idx < mBoard.size());

    if (mReadDirection == ReadDirection::topToBottom) {
        return mBoard
            .fields[mStartPoint.x + (mStartPoint.y + idx) * mBoard.size()];
    }
    return mBoard.fields[mStartPoint.x - idx + (mStartPoint.y) * mBoard.size()];
}

void Row::insertFieldData(std::size_t idx, const Field &fieldData)
{
    if (fieldData.hasSkyscraper()) {
        if (getFieldRef(idx).hasSkyscraper()) {
            return;
        }
        getFieldRef(idx).setBitmask(fieldData.bitmask());
        insertSkyscraperNeighbourHandling(
            idx, getFieldRef(idx).skyscraper(mBoard.size()));
    }
    else {
        bool hasSkyscraperBefore = getFieldRef(idx).hasSkyscraper();
        getFieldRef(idx).insertNopes(fieldData);

        auto nopes = fieldData.nopes(mBoard.size());

        for (const auto &nope : nopes) {
            insertNopesNeighbourHandling(idx, nope, hasSkyscraperBefore);
        }
    }
}

void Row::insertSkyscraperNeighbourHandling(std::size_t idx, int skyscraper)
{
    if (hasOnlyOneNopeField()) {
        addLastMissingSkyscraper();
    }
    addNopesToAllNopeFields(skyscraper);

    if (mCrossingRows[idx]->hasOnlyOneNopeField()) {
        mCrossingRows[idx]->addLastMissingSkyscraper();
    }

    mCrossingRows[idx]->addNopesToAllNopeFields(skyscraper);
}

void Row::insertNopesNeighbourHandling(std::size_t idx, int nope,
                                       bool hadSkyscraperBefore)
{
    // skyscraper was added so we have to add nopes to the neighbours
    if (!hadSkyscraperBefore && getFieldRef(idx).hasSkyscraper()) {

        insertSkyscraperNeighbourHandling(
            idx, getFieldRef(idx).skyscraper(mBoard.size()));
    }

    if (onlyOneFieldWithoutNope(nope)) {
        insertSkyscraperToFirstFieldWithoutNope(nope);
    }

    if (mCrossingRows[idx]->onlyOneFieldWithoutNope(nope)) {
        mCrossingRows[idx]->insertSkyscraperToFirstFieldWithoutNope(nope);
    }
}

bool Row::onlyOneFieldWithoutNope(int nope) const
{
    if (nopeExistsAsSkyscraperInFields(nope)) {
        return false;
    }
    if (nopeCount(nope) <
        static_cast<int>(mBoard.size()) - skyscraperCount() - 1) {
        return false;
    }
    return true;
}

bool Row::nopeExistsAsSkyscraperInFields(int nope) const
{
    for (std::size_t idx = 0; idx < mBoard.size(); ++idx) {
        if (getFieldRef(idx).skyscraper(mBoard.size()) == nope) {
            return true;
        }
    }
    return false;
}

std::optional<int> Row::nopeValueInAllButOneField() const
{
    std::unordered_map<int, int> nopeAndCount;

    for (std::size_t i = 0; i < mBoard.size(); ++i) {
        if (!getFieldRef(i).hasSkyscraper()) {
            auto nopes = getFieldRef(i).nopes(mBoard.size());
            for (const auto &nope : nopes) {
                if (hasSkyscraper(nope)) {
                    continue;
                }
                ++nopeAndCount[nope];
            }
        }
    }

    for (auto cit = nopeAndCount.cbegin(); cit != nopeAndCount.end(); ++cit) {
        if (cit->second ==
            static_cast<int>(mBoard.size()) - skyscraperCount() - 1) {
            return {cit->first};
        }
    }
    return {};
}

void Row::insertSkyscraperToFirstFieldWithoutNope(int nope)
{
    for (std::size_t idx = 0; idx < mBoard.size(); ++idx) {
        if ((getFieldRef(idx)).hasSkyscraper()) {
            continue;
        }
        if (!(getFieldRef(idx)).containsNope(nope)) {
            (getFieldRef(idx).insertSkyscraper(nope));
            insertSkyscraperNeighbourHandling(idx, nope);
            return; // there can be max one skyscraper per row;
        }
    }
}

bool Row::hasSkyscraper(int skyscraper) const
{
    for (std::size_t i = 0; i < mBoard.size(); ++i) {
        if (getFieldRef(i).skyscraper(mBoard.size()) == skyscraper) {
            return true;
        }
    }
    return false;
}

Field &Row::getFieldRef(std::size_t idx)
{
    assert(idx >= 0 && idx < mBoard.size());

    if (mReadDirection == ReadDirection::topToBottom) {
        return mBoard
            .fields[mStartPoint.x + (mStartPoint.y + idx) * mBoard.size()];
    }
    return mBoard.fields[mStartPoint.x - idx + (mStartPoint.y) * mBoard.size()];
}
