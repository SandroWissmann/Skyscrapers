#include "codewarsbacktracking.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_set>

namespace codewarsbacktracking {

struct ClueHints {
    ClueHints(std::size_t boardSize);
    ClueHints();

    void reverse();

    void removeNopesOnSkyscrapers();

    std::vector<int> skyscrapers{};
    std::vector<std::vector<int>> nopes{};
};

ClueHints::ClueHints(std::size_t boardSize)
    : skyscrapers{std::vector<int>(boardSize, 0)},
      nopes{std::vector<std::vector<int>>(boardSize, std::vector<int>{})}
{
}

void ClueHints::reverse()
{
    std::reverse(skyscrapers.begin(), skyscrapers.end());
    std::reverse(nopes.begin(), nopes.end());
}

void ClueHints::removeNopesOnSkyscrapers()
{
    for (std::size_t i = 0; i < skyscrapers.size(); ++i) {
        if (skyscrapers[i] == 0) {
            continue;
        }
        nopes[i].clear();
    }
}

std::optional<ClueHints> getClueHints(int clue, std::size_t boardSize)
{
    if (clue == 0) {
        return {};
    }

    ClueHints clueHints{boardSize};

    std::vector<std::unordered_set<int>> nopes(boardSize,
                                               std::unordered_set<int>{});

    if (clue == static_cast<int>(boardSize)) {
        for (std::size_t i = 0; i < boardSize; ++i) {
            clueHints.skyscrapers[i] = i + 1;
        }
    }
    else if (clue == 1) {
        clueHints.skyscrapers[0] = boardSize;
    }
    else if (clue == 2) {
        nopes[0].insert(boardSize);
        nopes[1].insert(boardSize - 1);
    }
    else {
        for (std::size_t fieldIdx = 0;
             fieldIdx < static_cast<std::size_t>(clue - 1); ++fieldIdx) {

            for (std::size_t nopeValue = boardSize;
                 nopeValue >= (boardSize - (clue - 2) + fieldIdx);
                 --nopeValue) {
                nopes[fieldIdx].insert(nopeValue);
            }
        }
    }

    assert(nopes.size() == clueHints.nopes.size());

    for (std::size_t i = 0; i < nopes.size(); ++i) {
        clueHints.nopes[i] = std::vector<int>(nopes[i].begin(), nopes[i].end());
    }
    return {clueHints};
}

std::optional<ClueHints> merge(std::optional<ClueHints> optFrontClueHints,
                               std::optional<ClueHints> optBackClueHints)
{
    if (!optFrontClueHints && !optBackClueHints) {
        return {};
    }
    if (!optFrontClueHints) {
        optBackClueHints->reverse();
        return optBackClueHints;
    }
    if (!optBackClueHints) {
        return optFrontClueHints;
    }

    auto size = optFrontClueHints->skyscrapers.size();
    ClueHints clueHints{size};

    assert(optFrontClueHints->skyscrapers.size() ==
           optFrontClueHints->nopes.size());
    assert(optBackClueHints->skyscrapers.size() ==
           optBackClueHints->nopes.size());
    assert(optFrontClueHints->skyscrapers.size() ==
           optBackClueHints->skyscrapers.size());

    optBackClueHints->reverse();

    for (std::size_t i = 0; i < optFrontClueHints->skyscrapers.size(); ++i) {

        auto frontSkyscraper = optFrontClueHints->skyscrapers[i];
        auto backSkyscraper = optBackClueHints->skyscrapers[i];

        if (frontSkyscraper != 0 && backSkyscraper != 0) {
            assert(frontSkyscraper == backSkyscraper);
            clueHints.skyscrapers[i] = frontSkyscraper;
        }
        else if (frontSkyscraper != 0) {
            clueHints.skyscrapers[i] = frontSkyscraper;
            clueHints.nopes[i].clear();
        }
        else { // backSkyscraper != 0
            clueHints.skyscrapers[i] = backSkyscraper;
            clueHints.nopes[i].clear();
        }

        if (clueHints.skyscrapers[i] != 0) {
            continue;
        }

        std::unordered_set<int> nopes(optFrontClueHints->nopes[i].begin(),
                                      optFrontClueHints->nopes[i].end());
        nopes.insert(optBackClueHints->nopes[i].begin(),
                     optBackClueHints->nopes[i].end());
        clueHints.nopes[i] = std::vector<int>(nopes.begin(), nopes.end());
    }
    clueHints.removeNopesOnSkyscrapers();
    return {clueHints};
}

void mergeClueHintsPerRow(std::vector<std::optional<ClueHints>> &clueHints)
{
    std::size_t startOffset = clueHints.size() / 4 * 3 - 1;
    std::size_t offset = startOffset;

    for (std::size_t frontIdx = 0; frontIdx < clueHints.size() / 2;
         ++frontIdx, offset -= 2) {

        if (frontIdx == clueHints.size() / 4) {
            offset = startOffset;
        }

        int backIdx = frontIdx + offset;

        clueHints[frontIdx] = merge(clueHints[frontIdx], clueHints[backIdx]);
    }
    clueHints.erase(clueHints.begin() + clueHints.size() / 2, clueHints.end());
}

std::vector<std::optional<ClueHints>>
getClueHints(const std::vector<int> &clues, std::size_t boardSize)
{
    std::vector<std::optional<ClueHints>> clueHints;
    clueHints.reserve(clues.size());

    for (const auto &clue : clues) {
        clueHints.emplace_back(getClueHints(clue, boardSize));
    }
    mergeClueHintsPerRow(clueHints);
    return clueHints;
}

template <typename It> int missingNumberInSequence(It begin, It end)
{
    int n = std::distance(begin, end) + 1;
    double projectedSum = (n + 1) * (n / 2.0);
    int actualSum = std::accumulate(begin, end, 0);
    return projectedSum - actualSum;
}

using BitmaskType = std::uint32_t;

/*
Example size = 4

b0000 0 Nopes = {}
b0001 1 Nopes = 1
b0010 2 Nopes = 2
b0011 3 Nopes = 1, 2
b0100 4 Nopes = 3
b0101 5 Nopes = 1, 3
b0110 6 Nopes = 2, 3
b0111 7 Skyscraper = 4
b1000 8 Nopes = 4
b1001 9 Nopes = 1, 4
b1010 10 Nopes = 2, 4
b1011 11 Skyscraper = 3
b1100 12 Nopes = 3, 4
b1101 13 Skyscraper
b1110 14 Skyscraper = 1
b1111 15 Invalid all nope
*/

class Field {
public:
    Field(std::size_t size);

    void insertSkyscraper(int skyscraper);
    void insertNope(int nope);
    void insertNopes(const std::vector<int> &nopes);

    int skyscraper() const;
    std::vector<int> nopes() const;

    bool hasSkyscraper() const;

    bool containsNope(int value) const;
    bool containsNopes(const std::vector<int> &values);

    BitmaskType bitmask() const;
    void setBitmask(BitmaskType bitmask);

private:
    bool bitIsToggled(BitmaskType bitmask, int bit) const;

    BitmaskType mBitmask{0};
    std::size_t mSize;

    friend inline bool operator==(const Field &lhs, const Field &rhs);
    friend inline bool operator!=(const Field &lhs, const Field &rhs);
};

inline bool operator==(const Field &lhs, const Field &rhs)
{
    return lhs.mBitmask == rhs.mBitmask;
}
inline bool operator!=(const Field &lhs, const Field &rhs)
{
    return !(lhs == rhs);
}

Field::Field(std::size_t size) : mSize{size}
{
}

void Field::insertSkyscraper(int skyscraper)
{
    assert(skyscraper > 0 && skyscraper <= static_cast<int>(mSize));
    BitmaskType mask = 1;
    for (int i = 0; i < static_cast<int>(mSize); ++i) {
        if (i != skyscraper - 1) {
            mBitmask |= mask;
        }
        mask <<= 1;
    }
}

void Field::insertNope(int nope)
{
    assert(nope > 0 && nope <= static_cast<int>(mSize));
    mBitmask |= 1 << (nope - 1);
}

void Field::insertNopes(const std::vector<int> &nopes)
{
    for (const auto nope : nopes) {
        insertNope(nope);
    }
}

int Field::skyscraper() const
{
    if (!hasSkyscraper()) {
        return 0;
    }

    for (std::size_t i = 0; i < mSize; ++i) {
        if (!(bitIsToggled(mBitmask, i))) {
            return i + 1;
        }
    }
    return 0;
}

std::vector<int> Field::nopes() const
{
    std::vector<int> nopes;
    nopes.reserve(mSize - 1);
    for (std::size_t i = 0; i < mSize; ++i) {
        if (bitIsToggled(mBitmask, i)) {
            nopes.emplace_back(i + 1);
        }
    }
    return nopes;
}

bool Field::hasSkyscraper() const
{
    bool foundZero = false;
    for (std::size_t i = 0; i < mSize; ++i) {
        if (!(bitIsToggled(mBitmask, i))) {
            if (!foundZero) {
                foundZero = true;
            }
            else { // found more than one zero so no skyscraper present
                return false;
            }
        }
    }
    return true;
}

bool Field::containsNope(int value) const
{
    return bitIsToggled(mBitmask, value - 1);
}

bool Field::containsNopes(const std::vector<int> &values)
{
    for (const auto &value : values) {
        if (!containsNope(value)) {
            return false;
        }
    }
    return true;
}

BitmaskType Field::bitmask() const
{
    return mBitmask;
}

void Field::setBitmask(BitmaskType bitmask)
{
    mBitmask = bitmask;
}

bool Field::bitIsToggled(BitmaskType bitmask, int bit) const
{
    return bitmask & (1 << bit);
}

struct Point {
    int x;
    int y;
};

inline bool operator==(const Point &lhs, const Point &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
inline bool operator!=(const Point &lhs, const Point &rhs)
{
    return !(lhs == rhs);
}

enum class ReadDirection { topToBottom, rightToLeft };

void nextDirection(ReadDirection &readDirection);

void advanceToNextPosition(Point &point, ReadDirection readDirection,
                           int clueIdx);

void nextDirection(ReadDirection &readDirection)
{
    assert(readDirection != ReadDirection::rightToLeft);
    int dir = static_cast<int>(readDirection);
    ++dir;
    readDirection = static_cast<ReadDirection>(dir);
}

void advanceToNextPosition(Point &point, ReadDirection readDirection,
                           int clueIdx)
{
    if (clueIdx == 0) {
        return;
    }
    switch (readDirection) {
    case ReadDirection::topToBottom:
        ++point.x;
        break;
    case ReadDirection::rightToLeft:
        ++point.y;
        break;
    }
}

class Row {
public:
    Row(std::vector<Field> &fields, std::size_t size, const Point &startPoint,
        const ReadDirection &readDirection);

    void insertSkyscraper(int pos, int skyscraper);

    std::size_t size() const;

    void addCrossingRows(Row *crossingRow);

    bool hasOnlyOneNopeField() const;
    void addLastMissingSkyscraper();

    void addNopesToAllNopeFields(int nope);

    bool allFieldsContainSkyscraper() const;

    int skyscraperCount() const;
    int nopeCount(int nope) const;

    void guessSkyscraperOutOfNeighbourNopes();

    enum class Direction { front, back };

    bool hasSkyscrapers(const std::vector<int> &skyscrapers,
                        Direction direction) const;
    bool hasNopes(const std::vector<std::vector<int>> &nopes,
                  Direction direction) const;

    void addSkyscrapers(const std::vector<int> &skyscrapers,
                        Direction direction);
    void addNopes(const std::vector<std::vector<int>> &nopes,
                  Direction direction);

    std::vector<Field *> getFields() const;

private:
    template <typename SkyIterator, typename FieldIterator>
    bool hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                        FieldIterator fieldItBegin,
                        FieldIterator fieldItEnd) const;

    template <typename NopesIterator, typename FieldIterator>
    bool hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                  FieldIterator fieldItBegin, FieldIterator fieldItEnd) const;

    template <typename SkyIterator, typename FieldIterator>
    void addSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                        FieldIterator fieldItBegin, FieldIterator fieldItEnd);

    template <typename NopesIterator, typename FieldIterator>
    void addNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                  FieldIterator fieldItBegin, FieldIterator fieldItEnd);

    template <typename IteratorType>
    void insertSkyscraper(IteratorType it, int skyscraper);

    template <typename IteratorType> void insertNope(IteratorType it, int nope);

    template <typename IteratorType>
    void insertNopes(IteratorType it, const std::vector<int> &nopes);

    int getIdx(std::vector<Field *>::const_iterator cit) const;
    int getIdx(std::vector<Field *>::const_reverse_iterator crit) const;

    std::vector<Field *> getRowFields(const ReadDirection &readDirection,
                                      std::vector<Field> &boardFields,
                                      std::size_t size,
                                      const Point &startPoint);

    bool onlyOneFieldWithoutNope(int nope) const;

    bool nopeExistsAsSkyscraperInFields(const std::vector<Field *> &rowFields,
                                        int nope) const;

    std::optional<int> nopeValueInAllButOneField() const;

    void insertSkyscraperToFirstFieldWithoutNope(int nope);

    bool hasSkyscraper(int skyscraper) const;

    std::vector<Row *> mCrossingRows;
    std::vector<Field *> mRowFields;
};

Row::Row(std::vector<Field> &fields, std::size_t size, const Point &startPoint,
         const ReadDirection &readDirection)
    : mRowFields{getRowFields(readDirection, fields, size, startPoint)}
{
}

void Row::insertSkyscraper(int pos, int skyscraper)
{
    assert(pos >= 0 && pos < static_cast<int>(mRowFields.size()));
    assert(skyscraper > 0 && skyscraper <= static_cast<int>(mRowFields.size()));
    auto it = mRowFields.begin() + pos;
    insertSkyscraper(it, skyscraper);
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
            sequence.emplace_back((*it)->skyscraper());
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
    insertSkyscraper(nopeFieldIt, missingValue);
}

void Row::addNopesToAllNopeFields(int nope)
{
    for (auto it = mRowFields.begin(); it != mRowFields.end(); ++it) {
        if ((*it)->hasSkyscraper()) {
            continue;
        }
        insertNope(it, nope);
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

void Row::addSkyscrapers(const std::vector<int> &skyscrapers,
                         Direction direction)
{
    if (direction == Direction::front) {
        addSkyscrapers(skyscrapers.begin(), skyscrapers.end(),
                       mRowFields.begin(), mRowFields.end());
    }
    else {
        addSkyscrapers(skyscrapers.begin(), skyscrapers.end(),
                       mRowFields.rbegin(), mRowFields.rend());
    }
}
void Row::addNopes(const std::vector<std::vector<int>> &nopes,
                   Direction direction)
{
    if (direction == Direction::front) {
        addNopes(nopes.begin(), nopes.end(), mRowFields.begin(),
                 mRowFields.end());
    }
    else {
        addNopes(nopes.begin(), nopes.end(), mRowFields.rbegin(),
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
        if ((*fieldIt)->skyscraper() != *skyIt) {
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

template <typename SkyIterator, typename FieldIterator>
void Row::addSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                         FieldIterator fieldItBegin, FieldIterator fieldItEnd)
{
    auto skyIt = skyItBegin;
    for (auto fieldIt = fieldItBegin;
         fieldIt != fieldItEnd && skyIt != skyItEnd; ++fieldIt, ++skyIt) {
        if (*skyIt == 0) {
            continue;
        }
        insertSkyscraper(fieldIt, *skyIt);
    }
}

template <typename NopesIterator, typename FieldIterator>
void Row::addNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                   FieldIterator fieldItBegin, FieldIterator fieldItEnd)
{
    auto nopesIt = nopesItBegin;
    for (auto fieldIt = fieldItBegin;
         fieldIt != fieldItEnd && nopesIt != nopesItEnd; ++fieldIt, ++nopesIt) {
        if (nopesIt->empty()) {
            continue;
        }
        insertNopes(fieldIt, *nopesIt);
    }
}

template <typename FieldIterator>
void Row::insertSkyscraper(FieldIterator fieldIt, int skyscraper)
{
    assert(mCrossingRows.size() == size());

    if ((*fieldIt)->hasSkyscraper()) {
        return;
    }
    (*fieldIt)->insertSkyscraper(skyscraper);

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
void Row::insertNope(FieldIterator fieldIt, int nope)
{
    if ((*fieldIt)->hasSkyscraper()) {
        return;
    }
    if ((*fieldIt)->containsNope(nope)) {
        return;
    }

    bool hasSkyscraperBefore = (*fieldIt)->hasSkyscraper();
    (*fieldIt)->insertNope(nope);

    // skyscraper was added so we have to add nopes to the neighbours
    // probaly could insert only nopes directly
    if (!hasSkyscraperBefore && (*fieldIt)->hasSkyscraper()) {
        insertSkyscraper(fieldIt, (*fieldIt)->skyscraper());
    }

    if (onlyOneFieldWithoutNope(nope)) {
        insertSkyscraperToFirstFieldWithoutNope(nope);
    }

    int idx = getIdx(fieldIt);

    if (mCrossingRows[idx]->onlyOneFieldWithoutNope(nope)) {
        mCrossingRows[idx]->insertSkyscraperToFirstFieldWithoutNope(nope);
    }
}

template <typename IteratorType>
void Row::insertNopes(IteratorType it, const std::vector<int> &nopes)
{
    for (const auto &nope : nopes) {
        insertNope(it, nope);
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
    auto cit = std::find_if(
        rowFields.cbegin(), rowFields.cend(),
        [nope](const auto &field) { return field->skyscraper() == nope; });
    return cit != rowFields.cend();
}

std::optional<int> Row::nopeValueInAllButOneField() const
{
    std::unordered_map<int, int> nopeAndCount;

    for (auto cit = mRowFields.cbegin(); cit != mRowFields.cend(); ++cit) {
        if (!(*cit)->hasSkyscraper()) {
            auto nopes = (*cit)->nopes();
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
            insertSkyscraper(it, nope);
            return; // there can be max one skyscraper per row;
        }
    }
}

bool Row::hasSkyscraper(int skyscraper) const
{
    for (const auto &field : mRowFields) {
        if (field->skyscraper() == skyscraper) {
            return true;
        }
    }
    return false;
}

class BorderIterator {
public:
    BorderIterator(std::size_t boardSize);

    Point point() const;
    ReadDirection readDirection() const;

    BorderIterator &operator++();

private:
    int mIdx = 0;
    std::size_t mBoardSize;
    Point mPoint{0, 0};
    ReadDirection mReadDirection{ReadDirection::topToBottom};
};

BorderIterator::BorderIterator(std::size_t boardSize) : mBoardSize{boardSize}
{
}

Point BorderIterator::point() const
{
    return mPoint;
}

ReadDirection BorderIterator::readDirection() const
{
    return mReadDirection;
}

BorderIterator &BorderIterator::operator++()
{
    ++mIdx;
    if (mIdx == static_cast<int>(2 * mBoardSize)) {
        return *this;
    }
    if (mIdx != 0 && mIdx % mBoardSize == 0) {
        nextDirection(mReadDirection);
    }

    advanceToNextPosition(mPoint, mReadDirection, mIdx % mBoardSize);
    return *this;
}

struct Board {
    Board(std::size_t size);

    void insert(const std::vector<std::optional<ClueHints>> &clueHints);

    void insert(const std::vector<std::vector<int>> &startingSkyscrapers);

    bool isSolved() const;

    std::vector<Field> fields;

    std::vector<Row> mRows;

    std::vector<std::vector<int>> skyscrapers2d() const;

    std::size_t size() const;

private:
    void makeRows();
    void connnectRowsWithCrossingRows();

    std::size_t mSize;
};

Board::Board(std::size_t size)
    : fields{std::vector<Field>(size * size, Field{size})}, mSize{size}
{
    makeRows();
}

void Board::insert(const std::vector<std::optional<ClueHints>> &clueHints)
{
    assert(clueHints.size() == mRows.size());

    for (std::size_t i = 0; i < clueHints.size(); ++i) {
        if (!clueHints[i]) {
            continue;
        }
        mRows[i].addNopes(clueHints[i]->nopes, Row::Direction::front);
        mRows[i].addSkyscrapers(clueHints[i]->skyscrapers,
                                Row::Direction::front);
    }
}

void Board::insert(const std::vector<std::vector<int>> &startingSkyscrapers)
{
    if (startingSkyscrapers.empty()) {
        return;
    }
    std::size_t boardSize = mRows.size() / 2;
    assert(startingSkyscrapers.size() == boardSize);

    for (std::size_t i = 0; i < startingSkyscrapers.size(); ++i) {
        mRows[i + boardSize].addSkyscrapers(startingSkyscrapers[i],
                                            Row::Direction::back);
    }
}

bool Board::isSolved() const
{
    std::size_t endVerticalRows = mRows.size() / 2;
    for (std::size_t i = 0; i < endVerticalRows; ++i) {
        if (!mRows[i].allFieldsContainSkyscraper()) {
            return false;
        }
    }
    return true;
}

std::vector<std::vector<int>> Board::skyscrapers2d() const
{
    std::vector<std::vector<int>> skyscrapers2d(mSize, std::vector<int>());

    std::size_t j = 0;
    skyscrapers2d[j].reserve(mSize);
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (i != 0 && i % mSize == 0) {
            ++j;
            skyscrapers2d[j].reserve(mSize);
        }
        skyscrapers2d[j].emplace_back(fields[i].skyscraper());
    }
    return skyscrapers2d;
}

std::size_t Board::size() const
{
    return mSize;
}

void Board::makeRows()
{
    BorderIterator borderIterator{mSize};

    std::size_t rowSize = mSize * 2;
    mRows.reserve(rowSize);

    for (std::size_t i = 0; i < rowSize; ++i, ++borderIterator) {
        mRows.emplace_back(Row{fields, mSize, borderIterator.point(),
                               borderIterator.readDirection()});
    }
    connnectRowsWithCrossingRows();
}

void Board::connnectRowsWithCrossingRows()
{
    std::size_t boardSize = mRows.size() / 2;

    std::vector<int> targetRowsIdx(boardSize);
    std::iota(targetRowsIdx.begin(), targetRowsIdx.end(), boardSize);

    for (std::size_t i = 0; i < mRows.size(); ++i) {
        if (i == mRows.size() / 2) {
            std::iota(targetRowsIdx.begin(), targetRowsIdx.end(), 0);
            std::reverse(targetRowsIdx.begin(), targetRowsIdx.end());
        }

        for (const auto &targetRowIdx : targetRowsIdx) {
            mRows[i].addCrossingRows(&mRows[targetRowIdx]);
        }
    }
}

void debug_print(Board &board, const std::string &title)
{
    std::cout << title << '\n';

    for (std::size_t i = 0; i < board.fields.size(); ++i) {

        if (i % board.size() == 0 && i != 0) {
            std::cout << '\n';
        }

        if (board.fields[i].skyscraper() != 0) {
            std::cout << std::setw(board.size() * 2);
            std::cout << "V" + std::to_string(board.fields[i].skyscraper());
        }
        else if (board.fields[i].skyscraper() == 0 &&
                 !board.fields[i].nopes().empty()) {
            auto nopes_set = board.fields[i].nopes();
            std::vector<int> nopes(nopes_set.begin(), nopes_set.end());
            std::sort(nopes.begin(), nopes.end());

            std::string nopesStr;
            for (std::size_t i = 0; i < nopes.size(); ++i) {
                nopesStr.append(std::to_string(nopes[i]));
                if (i != nopes.size() - 1) {
                    nopesStr.push_back(',');
                }
            }
            std::cout << std::setw(board.size() * 2);
            std::cout << nopesStr;
        }
        else {
            std::cout << ' ';
        }
    }
    std::cout << '\n';
}

template <typename FieldIterator>
int visibleBuildings(FieldIterator begin, FieldIterator end)
{
    int visibleBuildingsCount = 0;
    int highestSeen = 0;
    for (auto it = begin; it != end; ++it) {
        if (it->skyscraper() != 0 && it->skyscraper() > highestSeen) {
            ++visibleBuildingsCount;
            highestSeen = it->skyscraper();
        }
    }
    return visibleBuildingsCount;
}

bool rowsAreValid(const std::vector<Field> &fields, std::size_t index,
                  std::size_t rowSize)
{
    std::size_t row = index / rowSize;
    for (std::size_t currIndex = row * rowSize; currIndex < (row + 1) * rowSize;
         ++currIndex) {
        if (currIndex == index) {
            continue;
        }
        if (fields[currIndex].skyscraper() == fields[index].skyscraper()) {
            return false;
        }
    }
    return true;
}

bool columnsAreValid(const std::vector<Field> &fields, std::size_t index,
                     std::size_t rowSize)
{
    std::size_t column = index % rowSize;

    for (std::size_t i = 0; i < rowSize; ++i) {
        std::size_t currIndex = column + i * rowSize;
        if (currIndex == index) {
            continue;
        }
        if (fields[currIndex].skyscraper() == fields[index].skyscraper()) {
            return false;
        }
    }
    return true;
}

std::tuple<int, int> getRowClues(const std::vector<int> &clues, std::size_t row,
                                 std::size_t rowSize)
{
    int frontClue = clues[clues.size() - 1 - row];
    int backClue = clues[rowSize + row];
    return {frontClue, backClue};
}

bool rowCluesAreValid(const std::vector<Field> &fields,
                      const std::vector<int> &clues, std::size_t index,
                      std::size_t rowSize)
{
    std::size_t row = index / rowSize;

    auto [frontClue, backClue] = getRowClues(clues, row, rowSize);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::size_t rowIndexBegin = row * rowSize;
    std::size_t rowIndexEnd = (row + 1) * rowSize;

    auto citBegin = fields.cbegin() + rowIndexBegin;
    auto citEnd = fields.cbegin() + rowIndexEnd;

    bool rowIsFull = std::find_if(citBegin, citEnd, [](const Field &field) {
                         return !field.hasSkyscraper();
                     }) == citEnd;

    if (!rowIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible = visibleBuildings(citBegin, citEnd);

        if (frontClue != frontVisible) {
            return false;
        }
    }

    auto critBegin = std::make_reverse_iterator(citEnd);
    auto critEnd = std::make_reverse_iterator(citBegin);

    if (backClue != 0) {
        auto backVisible = visibleBuildings(critBegin, critEnd);

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

std::tuple<int, int> getColumnClues(const std::vector<int> &clues,
                                    std::size_t x, std::size_t size)
{
    int frontClue = clues[x];
    int backClue = clues[size * 3 - 1 - x];
    return {frontClue, backClue};
}

bool columnCluesAreValid(const std::vector<Field> &fields,
                         const std::vector<int> &clues, std::size_t index,
                         std::size_t rowSize)
{
    std::size_t column = index % rowSize;

    auto [frontClue, backClue] = getColumnClues(clues, column, rowSize);

    if (frontClue == 0 && backClue == 0) {
        return true;
    }

    std::vector<Field> verticalFields;
    verticalFields.reserve(rowSize);

    for (std::size_t i = 0; i < rowSize; ++i) {
        verticalFields.emplace_back(fields[column + i * rowSize]);
    }

    bool columnIsFull =
        std::find_if(verticalFields.cbegin(), verticalFields.cend(),
                     [](const Field &field) {
                         return !field.hasSkyscraper();
                     }) == verticalFields.cend();

    if (!columnIsFull) {
        return true;
    }

    if (frontClue != 0) {
        auto frontVisible =
            visibleBuildings(verticalFields.cbegin(), verticalFields.cend());
        if (frontClue != frontVisible) {
            return false;
        }
    }
    if (backClue != 0) {
        auto backVisible =
            visibleBuildings(verticalFields.crbegin(), verticalFields.crend());

        if (backClue != backVisible) {
            return false;
        }
    }
    return true;
}

bool skyscrapersAreValidPositioned(const std::vector<Field> &fields,
                                   const std::vector<int> &clues,
                                   std::size_t index, std::size_t rowSize)
{
    if (!rowsAreValid(fields, index, rowSize)) {
        return false;
    }
    if (!columnsAreValid(fields, index, rowSize)) {
        return false;
    }
    if (!rowCluesAreValid(fields, clues, index, rowSize)) {
        return false;
    }
    if (!columnCluesAreValid(fields, clues, index, rowSize)) {
        return false;
    }
    return true;
}

bool guessSkyscrapers(Board &board, const std::vector<int> &clues,
                      std::size_t index, std::size_t countOfElements,
                      std::size_t rowSize)
{
    if (index == countOfElements) {
        return true;
    }

    if (board.fields[index].skyscraper() != 0) {
        if (!skyscrapersAreValidPositioned(board.fields, clues, index,
                                           rowSize)) {
            return false;
        }
        if (guessSkyscrapers(board, clues, index + 1, countOfElements,
                             rowSize)) {
            return true;
        }
        return false;
    }

    auto oldBitmask = board.fields[index].bitmask();
    for (int trySkyscraper = 1; trySkyscraper <= static_cast<int>(rowSize);
         ++trySkyscraper) {

        if (board.fields[index].containsNope(trySkyscraper)) {
            continue;
        }
        board.fields[index].insertSkyscraper(trySkyscraper);
        if (!skyscrapersAreValidPositioned(board.fields, clues, index,
                                           rowSize)) {
            board.fields[index].setBitmask(oldBitmask);
            continue;
        }
        if (guessSkyscrapers(board, clues, index + 1, countOfElements,
                             rowSize)) {
            return true;
        }
        board.fields[index].setBitmask(oldBitmask);
    }
    board.fields[index].setBitmask(oldBitmask);
    return false;
}

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    std::size_t boardSize = clues.size() / 4;

    auto clueHints = getClueHints(clues, boardSize);

    Board board{boardSize};

    board.insert(clueHints);
    board.insert(startingGrid);

    if (board.isSolved()) {
        return board.skyscrapers2d();
    }
    guessSkyscrapers(board, clues, 0, board.fields.size(), board.size());

    return board.skyscrapers2d();
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace codewarsbacktracking
