#include "codewarspermutation.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace codewarspermutation {

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

class Nopes {
public:
    Nopes(int size);

    void insert(int value);
    void insert(const std::vector<int> &values);
    bool sizeReached() const;
    int missingNumberInSequence() const;

    bool contains(int value) const;
    bool contains(const std::vector<int> &values);

    bool isEmpty() const;
    void clear();

    std::vector<int> containing() const;

    // for debug print
    std::unordered_set<int> values() const;

private:
    int mSize;
    std::unordered_set<int> mValues;
};

Nopes::Nopes(int size) : mSize{size}
{
    assert(size > 0);
}

void Nopes::insert(int value)
{
    assert(value >= 1 && value <= mSize + 1);
    mValues.insert(value);
}

void Nopes::insert(const std::vector<int> &values)
{
    mValues.insert(values.begin(), values.end());
}

bool Nopes::sizeReached() const
{
    return mValues.size() == static_cast<std::size_t>(mSize);
}

int Nopes::missingNumberInSequence() const
{
    assert(sizeReached());
    return codewarspermutation::missingNumberInSequence(mValues.begin(),
                                                        mValues.end());
}

bool Nopes::contains(int value) const
{
    auto it = mValues.find(value);
    return it != mValues.end();
}

bool Nopes::contains(const std::vector<int> &values)
{
    for (const auto &value : values) {
        if (!contains(value)) {
            return false;
        }
    }
    return true;
}

bool Nopes::isEmpty() const
{
    return mValues.empty();
}

void Nopes::clear()
{
    mValues.clear();
}

std::vector<int> Nopes::containing() const
{
    std::vector<int> nopes;
    nopes.reserve(mValues.size());
    for (const auto &value : mValues) {
        nopes.emplace_back(value);
    }
    return nopes;
}

std::unordered_set<int> Nopes::values() const
{
    return mValues;
}

class Field {
public:
    Field(int &skyscraper, Nopes &nopes);

    void insertSkyscraper(int skyscraper);
    void insertNope(int nope);
    void insertNopes(const std::vector<int> &nopes);

    bool fullOfNopes() const;

    int skyscraper() const;
    Nopes nopes() const;

    bool hasSkyscraper() const;

    std::optional<int> lastMissingNope() const;

private:
    int &mSkyscraper;
    Nopes &mNopes;
    bool mHasSkyscraper = false;
};

Field::Field(int &skyscraper, Nopes &nopes)
    : mSkyscraper{skyscraper}, mNopes{nopes}
{
}

void Field::insertSkyscraper(int skyscraper)
{
    assert(mSkyscraper == 0 || skyscraper == mSkyscraper);
    if (mHasSkyscraper) {
        return;
    }
    mSkyscraper = skyscraper;
    mHasSkyscraper = true;

    mNopes.clear();
}
void Field::insertNope(int nope)
{
    if (mHasSkyscraper) {
        return;
    }
    mNopes.insert(nope);
}
void Field::insertNopes(const std::vector<int> &nopes)
{
    if (mHasSkyscraper) {
        return;
    }
    mNopes.insert(nopes);
}

bool Field::fullOfNopes() const
{
    return mNopes.sizeReached();
}

int Field::skyscraper() const
{
    return mSkyscraper;
}
Nopes Field::nopes() const
{
    return mNopes;
}

bool Field::hasSkyscraper() const
{
    return mHasSkyscraper;
}

std::optional<int> Field::lastMissingNope() const
{
    if (!mNopes.sizeReached()) {
        return {};
    }
    return mNopes.missingNumberInSequence();
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
    Row(std::vector<std::vector<Field>> &fields, const Point &startPoint,
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
                                      std::vector<std::vector<Field>> &fields,
                                      const Point &startPoint);

    bool onlyOneFieldWithoutNope(int nope) const;
    std::optional<int> nopeValueInAllButOneField() const;

    void insertSkyscraperToFirstFieldWithoutNope(int nope);

    bool hasSkyscraper(int skyscraper) const;

    std::vector<Row *> mCrossingRows;
    std::vector<Field *> mRowFields;
};

Row::Row(std::vector<std::vector<Field>> &fields, const Point &startPoint,
         const ReadDirection &readDirection)
    : mRowFields{getRowFields(readDirection, fields, startPoint)}
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
        if ((*cit)->nopes().contains(nope)) {
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
        if (!(*fieldIt)->nopes().contains(*nopesIt)) {
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
    if ((*fieldIt)->nopes().contains(nope)) {
        return;
    }
    (*fieldIt)->insertNope(nope);

    auto optlastMissingNope = (*fieldIt)->lastMissingNope();
    if (optlastMissingNope) {
        insertSkyscraper(fieldIt, *optlastMissingNope);
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

std::vector<Field *>
Row::getRowFields(const ReadDirection &readDirection,
                  std::vector<std::vector<Field>> &boardFields,
                  const Point &startPoint)
{
    std::vector<Field *> fields;
    fields.reserve(boardFields.size());
    std::size_t x = startPoint.x;
    std::size_t y = startPoint.y;
    for (std::size_t i = 0; i < boardFields.size(); ++i) {
        fields.emplace_back(&boardFields[y][x]);

        if (readDirection == ReadDirection::topToBottom) {
            ++y;
        }
        else {
            --x;
        }
    }
    return fields;
}

bool Row::onlyOneFieldWithoutNope(int nope) const
{
    auto cit = std::find_if(
        mRowFields.cbegin(), mRowFields.cend(),
        [nope](const auto &field) { return field->skyscraper() == nope; });
    if (cit != mRowFields.cend()) {
        return false;
    }
    if (nopeCount(nope) < static_cast<int>(size()) - skyscraperCount() - 1) {
        return false;
    }
    return true;
}

std::optional<int> Row::nopeValueInAllButOneField() const
{
    std::unordered_map<int, int> nopeAndCount;

    for (auto cit = mRowFields.cbegin(); cit != mRowFields.cend(); ++cit) {
        if (!(*cit)->hasSkyscraper()) {
            auto nopes = (*cit)->nopes().containing();
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
        if (!(*it)->nopes().contains(nope)) {
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

    std::vector<std::vector<int>> skyscrapers{};
    std::vector<std::vector<Nopes>> nopes;

    std::vector<Row> mRows;

private:
    std::vector<std::vector<int>> makeSkyscrapers(std::size_t size);
    std::vector<std::vector<Nopes>> makeNopes(std::size_t size);

    void makeFields();
    void makeRows();
    void connnectRowsWithCrossingRows();

    std::vector<std::vector<Field>> mFields;
};

Board::Board(std::size_t size)
    : skyscrapers{makeSkyscrapers(size)}, nopes{makeNopes(size)},
      mFields{std::vector<std::vector<Field>>(skyscrapers.size())}
{
    makeFields();
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

std::vector<std::vector<int>> Board::makeSkyscrapers(std::size_t size)
{
    std::vector<int> skyscraperRow(size, 0);
    return std::vector<std::vector<int>>(size, skyscraperRow);
}

std::vector<std::vector<Nopes>> Board::makeNopes(std::size_t size)
{
    std::vector<Nopes> nopesRow(size, Nopes{static_cast<int>(size) - 1});
    return std::vector<std::vector<Nopes>>(size, nopesRow);
}

void Board::makeFields()
{
    mFields.reserve(skyscrapers.size());
    for (auto &row : mFields) {
        row.reserve(mFields.size());
    }
    for (std::size_t y = 0; y < skyscrapers.size(); ++y) {
        mFields[y].reserve(skyscrapers.size());
        for (std::size_t x = 0; x < skyscrapers[y].size(); ++x) {
            mFields[y].emplace_back(Field{skyscrapers[y][x], nopes[y][x]});
        }
    }
}

void Board::makeRows()
{
    BorderIterator borderIterator{mFields.size()};

    std::size_t size = mFields.size() * 2;
    mRows.reserve(size);

    for (std::size_t i = 0; i < size; ++i, ++borderIterator) {
        mRows.emplace_back(Row{mFields, borderIterator.point(),
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
    for (std::size_t y = 0; y < board.skyscrapers.size(); ++y) {
        for (std::size_t x = 0; x < board.skyscrapers[y].size(); ++x) {

            if (board.skyscrapers[y][x] != 0) {
                std::cout << std::setw(board.skyscrapers.size() * 2);
                std::cout << "V" + std::to_string(board.skyscrapers[y][x]);
            }
            else if (board.skyscrapers[y][x] == 0 &&
                     !board.nopes[y][x].isEmpty()) {
                auto nopes_set = board.nopes[y][x].values();
                std::vector<int> nopes(nopes_set.begin(), nopes_set.end());
                std::sort(nopes.begin(), nopes.end());

                std::string nopesStr;
                for (std::size_t i = 0; i < nopes.size(); ++i) {
                    nopesStr.append(std::to_string(nopes[i]));
                    if (i != nopes.size() - 1) {
                        nopesStr.push_back(',');
                    }
                }
                std::cout << std::setw(board.skyscrapers.size() * 2);
                std::cout << nopesStr;
            }
            else {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

class CluePair {
public:
    CluePair(int front, int back);

    int front() const;
    int back() const;

    bool frontIsEmpty() const;
    bool backIsEmpty() const;
    bool isEmpty() const;

private:
    int mFront;
    int mBack;
    bool mFrontIsEmpty;
    bool mBackIsEmpty;
    bool mIsEmpty;
};

CluePair::CluePair(int front, int back)
    : mFront{front}, mBack{back}, mFrontIsEmpty{mFront == 0},
      mBackIsEmpty{mBack == 0}, mIsEmpty{mFrontIsEmpty && mBackIsEmpty}
{
}

int CluePair::front() const
{
    return mFront;
}

int CluePair::back() const
{
    return mBack;
}

bool CluePair::frontIsEmpty() const
{
    return mFrontIsEmpty;
}

bool CluePair::backIsEmpty() const
{
    return mBackIsEmpty;
}

bool CluePair::isEmpty() const
{
    return mIsEmpty;
}

std::vector<CluePair> makeCluePairs(const std::vector<int> &clues)
{
    std::vector<CluePair> cluePairs;
    cluePairs.reserve(clues.size() / 2);

    std::size_t startOffset = clues.size() / 4 * 3 - 1;
    std::size_t offset = startOffset;

    for (std::size_t i = 0; i < clues.size() / 2; ++i, offset -= 2) {

        if (i == clues.size() / 4) {
            offset = startOffset;
        }

        int backClueIdx = i + offset;
        cluePairs.emplace_back(CluePair{clues[i], clues[backClueIdx]});
    }
    return cluePairs;
}

template <typename T> class Span {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using const_iterator = const_pointer;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    Span(const_pointer ptr, size_type size);

    constexpr const_pointer data() const noexcept;

    constexpr size_type size() const noexcept;

    constexpr const_reference operator[](size_type idx) const;

    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;

    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

private:
    const_pointer mPtr;
    size_type mSize;
};

template <typename T>
Span<T>::Span(const_pointer ptr, size_type size) : mPtr{ptr}, mSize{size}
{
}

template <typename T>
constexpr typename Span<T>::const_pointer Span<T>::data() const noexcept
{
    return mPtr;
}

template <typename T>
constexpr typename Span<T>::size_type Span<T>::size() const noexcept
{
    return mSize;
}

template <typename T>
constexpr typename Span<T>::const_reference
Span<T>::operator[](Span::size_type idx) const
{
    assert(idx < mSize);
    return *(data() + idx);
}

template <typename T>
constexpr typename Span<T>::const_iterator Span<T>::cbegin() const noexcept
{
    return data();
}

template <typename T>
constexpr typename Span<T>::const_iterator Span<T>::cend() const noexcept
{
    return data() + size();
}

template <typename T>
constexpr typename Span<T>::const_reverse_iterator
Span<T>::crbegin() const noexcept
{
    return reverse_iterator(cend());
}

template <typename T>
constexpr typename Span<T>::const_reverse_iterator
Span<T>::crend() const noexcept
{
    return reverse_iterator(cbegin());
}

class Permutations {
public:
    Permutations(std::size_t size, Span<CluePair> cluePairs, Span<Row> rows);

    Span<int> operator[](std::size_t permutationIndex) const;

    std::vector<std::size_t> permutationIndexs(std::size_t cluePairIndex) const;

private:
    void addSequenceToCluePairs(const std::vector<int> &sequence,
                                int currIndex);

    bool permutationFitsCluePair(const CluePair &cluePair, int front, int back);

    Span<CluePair> mCluePairs;
    Span<Row> mRows;

    std::size_t mSize;
    std::size_t mPermutationCount;
    std::vector<std::vector<std::size_t>> mCluePairsPermutationIndexes;

    std::vector<int> mPermutations;
};

template <typename BuildingIt>
int buildingsVisible(BuildingIt begin, BuildingIt end)
{
    int visibleBuildingsCount = 0;
    int highestSeen = 0;

    for (auto it = begin; it != end; ++it) {
        if (*it > highestSeen) {
            ++visibleBuildingsCount;
            highestSeen = *it;
        }
    }
    return visibleBuildingsCount;
}

int factorial(int n)
{
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

Permutations::Permutations(std::size_t size, Span<CluePair> cluePairs,
                           Span<Row> rows)
    : mCluePairs(cluePairs), mRows{rows}, mSize{size},
      mCluePairsPermutationIndexes(cluePairs.size())
{
    assert(cluePairs.size() == rows.size());
    std::vector<int> sequence(mSize);
    std::iota(sequence.begin(), sequence.end(), 1);
    mPermutationCount = factorial(sequence.size());

    mPermutations.reserve(mPermutationCount * mSize);
    int *p = &mPermutations[0];

    for (auto &cluePairPermutationIndexes : mCluePairsPermutationIndexes) {
        cluePairPermutationIndexes.reserve(mPermutationCount);
    }

    std::size_t currIndex = 0;
    do {
        addSequenceToCluePairs(sequence, currIndex);
        std::copy(sequence.begin(), sequence.end(), p);
        p += mSize;
        ++currIndex;
    } while (std::next_permutation(sequence.begin(), sequence.end()));
};

Span<int> Permutations::operator[](std::size_t elem) const
{
    auto ptr = &mPermutations[elem * mSize];
    return Span{ptr, mSize};
}

std::vector<std::size_t>
Permutations::permutationIndexs(std::size_t cluePairIndex) const
{
    return mCluePairsPermutationIndexes[cluePairIndex];
}

bool existingSkyscrapersInPermutation(const std::vector<Field *> &fields,
                                      const std::vector<int> &permutation)
{
    assert(fields.size() == permutation.size());

    auto fieldIt = fields.cbegin();
    auto permutationIt = permutation.cbegin();

    for (; fieldIt != fields.cend() && permutationIt != permutation.cend();
         ++fieldIt, ++permutationIt) {
        if (!(*fieldIt)->hasSkyscraper()) {
            continue;
        }
        if ((*fieldIt)->skyscraper() != *permutationIt) {
            return false;
        }
    }
    return true;
}

void Permutations::addSequenceToCluePairs(const std::vector<int> &sequence,
                                          int currIndex)
{
    auto front = buildingsVisible(sequence.cbegin(), sequence.cend());
    auto back = buildingsVisible(sequence.crbegin(), sequence.crend());

    for (std::size_t i = 0; i < mCluePairs.size(); ++i) {
        if (!permutationFitsCluePair(mCluePairs[i], front, back)) {
            continue;
        }
        auto fields = mRows[i].getFields();

        if (!existingSkyscrapersInPermutation(fields, sequence)) {
            continue;
        }
        mCluePairsPermutationIndexes[i].emplace_back(currIndex);
    }
}

bool Permutations::permutationFitsCluePair(const CluePair &cluePair, int front,
                                           int back)
{
    if (cluePair.isEmpty()) {
        return false;
    }
    if (!cluePair.frontIsEmpty() && cluePair.front() != front) {
        return false;
    }
    if (!cluePair.backIsEmpty() && cluePair.back() != back) {
        return false;
    }
    return true;
}

struct FieldElements {
    std::vector<int> skyscrapers{};
    std::vector<std::vector<int>> nopes{};
};

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
        if (lastIt->skyscraper() != (*currPtrIt)->skyscraper()) {
            return false;
        }
        if (lastIt->nopes().values() != (*currPtrIt)->nopes().values()) {
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
            if ((*fieldIt)->skyscraper() != *permIt) {
                return false;
            }
        }
        else if (!(*fieldIt)->nopes().isEmpty()) {
            if ((*fieldIt)->nopes().contains(*permIt)) {
                return false;
            }
        }
    }
    return true;
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
        return board.skyscrapers;
    }

    auto cluePairs = makeCluePairs(clues);
    Permutations permutations(boardSize, Span{&cluePairs[0], cluePairs.size()},
                              Span{&board.mRows[0], board.mRows.size()});

    std::vector<Slice> slices =
        makeSlices(permutations, board.mRows, cluePairs);

    for (;;) {
        bool allFull = true;
        for (std::size_t i = 0; i < slices.size(); ++i) {
            if (slices[i].isSolved()) {
                continue;
            }
            slices[i].solveFromPossiblePermutations();
            slices[i].guessSkyscraperOutOfNeighbourNopes();

            if (!slices[i].isSolved()) {
                allFull = false;
            }
        }

        if (allFull) {
            break;
        }
    }

    return board.skyscrapers;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}

} // namespace codewarspermutation
