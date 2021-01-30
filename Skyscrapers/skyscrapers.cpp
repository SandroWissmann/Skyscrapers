#include "skyscrapers.h"

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <unordered_set>

#include <chrono>
#include <iostream>

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
    int n = mValues.size() + 1;
    int projectedSum = (n + 1) * (n / 2);
    int actualSum = std::accumulate(mValues.begin(), mValues.end(), 0);
    return projectedSum - actualSum;
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

std::unordered_set<int> Nopes::values() const
{
    return mValues;
}

struct Point {
    int x;
    int y;
};

enum class BorderPosition { top, right, bottom, left };

void nextDirection(BorderPosition &borderPosition)
{
    if (borderPosition == BorderPosition::left) {
        borderPosition = BorderPosition::top;
    }
    else {
        int dir = static_cast<int>(borderPosition);
        ++dir;
        borderPosition = static_cast<BorderPosition>(dir);
    }
}

void advanceToNextPosition(Point &point, BorderPosition borderPosition,
                           int clueIdx)
{
    if (clueIdx == 0) {
        return;
    }
    switch (borderPosition) {
    case BorderPosition::top:
        ++point.x;
        break;
    case BorderPosition::right:
        ++point.y;
        break;
    case BorderPosition::bottom:
        --point.x;
        break;
    case BorderPosition::left:
        --point.y;
        break;
    }
}

class BorderIterator {
public:
    BorderIterator(std::size_t boardSize);

    Point point() const;
    BorderPosition borderPosition() const;

    BorderIterator &operator++();

private:
    int mIdx = 0;
    std::size_t mBoardSize;
    Point mPoint{0, 0};
    BorderPosition mReadDirection{BorderPosition::top};
};

BorderIterator::BorderIterator(std::size_t boardSize) : mBoardSize{boardSize}
{
}

Point BorderIterator::point() const
{
    return mPoint;
}

BorderPosition BorderIterator::borderPosition() const
{
    return mReadDirection;
}

BorderIterator &BorderIterator::operator++()
{
    ++mIdx;
    if (mIdx != 0 && mIdx % mBoardSize == 0) {
        nextDirection(mReadDirection);
    }

    advanceToNextPosition(mPoint, mReadDirection, mIdx % mBoardSize);
    return *this;
}

struct Board {
    Board(int size);

    std::vector<std::vector<int>> skyscrapers{};
    std::vector<std::vector<Nopes>> nopes;

private:
    std::vector<std::vector<int>> makeSkyscrapers(int size);
    std::vector<std::vector<Nopes>> makeNopes(int size);
};

Board::Board(int size)
    : skyscrapers{makeSkyscrapers(size)}, nopes{makeNopes(size)}
{
}

std::vector<std::vector<int>> Board::makeSkyscrapers(int size)
{
    std::vector<int> skyscraperRow(size, 0);
    return std::vector<std::vector<int>>(size, skyscraperRow);
}

std::vector<std::vector<Nopes>> Board::makeNopes(int size)
{
    std::vector<Nopes> nopesRow(size, Nopes{size - 1});
    return std::vector<std::vector<Nopes>>(size, nopesRow);
}

class Slice {
public:
    Slice(Board &board, const Point &startPoint, BorderPosition borderPosition,
          int clue);

    void insertSkyscraper(int skyscraper, std::size_t idx);
    void insertNopes(int nope, std::size_t idx);
    void insertNopes(const std::vector<int> &nopes, std::size_t idx);

    void addOpposideSlice(Slice *oppositeSlice);
    void addCrossingSlice(Slice *crossingSlice);

    bool skyscraperExists(std::size_t idx) const;

    int skyscraper(std::size_t idx) const;
    Nopes nopes(std::size_t idx) const;

    std::size_t size() const;

    int clue() const;

    bool isFull() const;

    void addNopesFromClue();
    void solveFromClue();
    void solveFromClueAndOpositeSliceClue();
    void solveFromNopes();

private:
    void solveFromNopes(int searchSkyscraper);

    bool hasExpectedSkyscrapers(const std::vector<int> &expectedSkyscrapers);
    bool hasExpectedNopes(const std::vector<std::vector<int>> &expectedNopes);
    void addNewSkyscrapers(const std::vector<int> &newSkyscrapers);
    void addNewNopes(const std::vector<std::vector<int>> &newNopes);

    void transformNopesToSkyscraperIfSizeReached(const Point &p, int idx);
    void insertLastMissingSkyscraper();
    void addMissingNopesFromSkyscrapers();

    Point calcPosition(int idx) const;

    Slice *mOppositeSlice;
    std::vector<Slice *> mCrossingSlices;
    Board &mBoard;
    Point mStartPoint;
    BorderPosition mDirection;
    int mClue;
    int mSkyscraperCount{0};
    std::size_t mBoardSize;
    std::vector<bool> mSkyscraperAtPos;
    mutable bool mIsfull{false};

    struct FieldsStates {
        std::vector<int> skyscrapers;
        std::vector<std::vector<int>> nopes;
    };
};

Slice::Slice(Board &board, const Point &startPoint,
             BorderPosition borderPosition, int clue)
    : mBoard{board}, mStartPoint{startPoint}, mDirection{borderPosition},
      mClue{clue}, mBoardSize{mBoard.skyscrapers.size()},
      mSkyscraperAtPos{std::vector<bool>(mBoardSize, false)}
{
    mCrossingSlices.reserve(mBoardSize);

    assert(mClue >= 0 && mClue <= static_cast<int>(mBoardSize));
}

void Slice::insertSkyscraper(int skyscraper, std::size_t idx)
{
    assert(mCrossingSlices.size() == mBoardSize || mCrossingSlices.empty());
    if (mSkyscraperAtPos[idx]) {
        return;
    }
    Point p = calcPosition(idx);
    mBoard.skyscrapers[p.y][p.x] = skyscraper;
    mSkyscraperAtPos[idx] = true;
    ++mSkyscraperCount;

    if (mSkyscraperCount == static_cast<int>(mBoardSize) - 1) {
        insertLastMissingSkyscraper();
    }
    else {
        addMissingNopesFromSkyscrapers();
    }
    if (!mCrossingSlices.empty()) {
        mCrossingSlices[idx]->addMissingNopesFromSkyscrapers();
    }
}

void Slice::insertNopes(int nope, std::size_t idx)
{
    if (mSkyscraperAtPos[idx]) {
        return;
    }
    Point p = calcPosition(idx);
    mBoard.nopes[p.y][p.x].insert(nope);
    transformNopesToSkyscraperIfSizeReached(p, idx);
}

void Slice::insertNopes(const std::vector<int> &nopes, std::size_t idx)
{
    if (mSkyscraperAtPos[idx]) {
        return;
    }
    Point p = calcPosition(idx);
    mBoard.nopes[p.y][p.x].insert(nopes);
    transformNopesToSkyscraperIfSizeReached(p, idx);
}

void Slice::addOpposideSlice(Slice *oppositeSlice)
{
    assert(oppositeSlice != nullptr);
    mOppositeSlice = oppositeSlice;
}

void Slice::addCrossingSlice(Slice *crossingSlice)
{
    assert(crossingSlice != nullptr);
    assert(mCrossingSlices.size() < mBoardSize);
    mCrossingSlices.push_back(crossingSlice);
}

bool Slice::skyscraperExists(std::size_t idx) const
{
    return mSkyscraperAtPos[idx];
}

int Slice::skyscraper(std::size_t idx) const
{
    Point p = calcPosition(idx);
    return mBoard.skyscrapers[p.y][p.x];
}

Nopes Slice::nopes(std::size_t idx) const
{
    Point p = calcPosition(idx);
    return mBoard.nopes[p.y][p.x];
}

std::size_t Slice::size() const
{
    return mBoardSize;
}

int Slice::clue() const
{
    return mClue;
}

bool Slice::isFull() const
{
    if (mIsfull) {
        return mIsfull;
    }
    for (std::size_t i = 0; i < size(); ++i) {
        if (skyscraper(i) == 0) {
            return false;
        }
    }
    mIsfull = true;
    return mIsfull;
}

void Slice::addNopesFromClue()
{
    if (mClue == 0) {
        return;
    }
    int size = mBoardSize;
    std::vector<std::vector<std::vector<int>>> clueWithNopes{
        {{size - 1, size - 2, size - 3, size - 4, size - 5},
         {size},
         {size},
         {size},
         {size},
         {size}},
        {{size}, {size - 1}},
        {{size, size - 1}, {size}},
        {{size, size - 1, size - 2}, {size, size - 1}, {size}},
        {{size, size - 1, size - 2, size - 3},
         {size, size - 1, size - 2},
         {size, size - 1},
         {size}},
        {
            {size, size - 1, size - 2, size - 3, size - 4},
            {size, size - 1, size - 2, size - 3, size - 5},
            {size, size - 1, size - 2, size - 4, size - 5},
            {size, size - 1, size - 3, size - 4, size - 5},
            {size, size - 2, size - 3, size - 4, size - 5},
            {size - 1, size - 2, size - 3, size - 4, size - 5},

        }};

    assert(clueWithNopes.size() == mBoardSize);

    for (std::size_t nopesIdx = 0; nopesIdx < clueWithNopes[mClue - 1].size();
         ++nopesIdx) {
        insertNopes(clueWithNopes[mClue - 1][nopesIdx], nopesIdx);
    }
}

void Slice::solveFromClue()
{
    std::vector<std::vector<FieldsStates>> clueFieldsStateCombinations{
        {},
        {},
        // 2
        {{{0, 3, 6, 2, 0, 5}, {{}, {}, {}, {}, {}, {}}},
         {{0, 0, 0, 3, 6, 5}, {{}, {}, {}, {}, {}, {}}},
         {{0, 0, 0, 6, 0, 0}, {{3, 5}, {}, {}, {}, {}, {}}},
         {{0, 0, 6, 0, 0, 0}, {{}, {4}, {}, {}, {}, {5}}},
         {{4, 0, 0, 6, 0, 0}, {{}, {}, {}, {}, {}, {}}},
         {{0, 0, 2, 6, 5, 4}, {{}, {}, {}, {}, {}, {}}},
         {{0, 0, 3, 0, 6, 5}, {{2}, {4}, {}, {}, {}, {}}}},
        // 3
        {{{0, 0, 0, 0, 5, 6}, {{}, {}, {}, {}, {}, {}}},
         {{0, 0, 5, 0, 6, 4}, {{}, {1}, {}, {}, {}, {}}},
         {{0, 0, 6, 5, 0, 0}, {{1}, {2}, {}, {}, {4}, {}}},
         {{0, 0, 6, 0, 5, 0}, {{1}, {4}, {}, {}, {}, {}}},
         {{0, 0, 0, 0, 6, 5}, {{}, {}, {}, {}, {}, {}}},
         {{0, 0, 6, 0, 0, 0}, {{}, {}, {}, {}, {4}, {5}}},
         {{0, 0, 0, 0, 0, 6}, {{4}, {1}, {5}, {}, {5}, {}}},
         {{0, 0, 0, 0, 6, 0}, {{4}, {}, {}, {1, 2, 5}, {}, {}}},
         {{0, 0, 0, 0, 0, 6}, {{}, {4, 5}, {}, {}, {}, {}}},
         {{0, 0, 0, 0, 0, 6}, {{1}, {4, 5}, {}, {}, {}, {}}},
         {{0, 0, 0, 0, 0, 6}, {{4}, {1, 3, 4}, {1, 5}, {}, {5}, {}}},
         {{0, 0, 0, 6, 0, 5}, {{}, {}, {3}, {}, {4}, {}}},
         {{0, 0, 0, 0, 0, 6}, {{1}, {4, 5}, {4, 5}, {}, {}, {}}},
         {{0, 0, 0, 0, 6, 0}, {{4}, {3, 5}, {}, {1, 2, 5}, {}, {}}},
         {{4, 0, 6, 0, 0, 0}, {{}, {1}, {}, {5}, {}, {5}}},
         {{0, 0, 0, 6, 0, 4}, {{}, {}, {5}, {}, {1}, {}}},
         {{0, 0, 0, 0, 0, 6}, {{1, 2}, {4, 5}, {4, 5}, {3}, {}, {}}},
         {{0, 0, 0, 6, 5, 4}, {{}, {2}, {}, {}, {}, {}}},
         {{0, 0, 5, 0, 6, 0}, {{4}, {3}, {}, {1, 2}, {}, {4}}}},
        // 4
        {{{0, 4, 2, 0, 0, 0}, {{}, {}, {}, {}, {5}, {6}}},
         {{0, 0, 5, 6, 4, 0}, {{}, {}, {}, {}, {}, {1}}},
         {{0, 0, 0, 4, 0, 6}, {{}, {}, {}, {}, {}, {1}}},
         {{0, 0, 0, 4, 5, 6}, {{}, {}, {}, {}, {}, {}}},
         {{2, 0, 0, 6, 5, 0}, {{}, {4}, {3}, {}, {}, {}}}},
        // 5
        {{{0, 0, 0, 0, 6, 0}, {{}, {}, {}, {}, {}, {}}},
         {{2, 0, 0, 5, 0, 6}, {{}, {}, {3}, {}, {}, {}}}},
        {}};

    std::vector<std::vector<std::vector<int>>> clueNewSkyscraperCombinations{
        {},
        {},
        // 2
        {{4, 0, 0, 0, 1, 0},
         {4, 0, 0, 0, 0, 0},
         {4, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {3, 1, 0, 0, 0, 0},
         {4, 0, 0, 0, 0, 0}},
        // 3
        {{4, 0, 0, 0, 0, 0},
         {3, 2, 0, 1, 0, 0},
         {0, 4, 0, 0, 0, 0},
         {2, 3, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 5, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 5, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0}},
        // 4
        {{0, 0, 0, 5, 6, 0},
         {1, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0},
         {3, 0, 0, 0, 0, 0},
         {0, 3, 4, 0, 0, 1}},
        // 5
        {{0, 0, 0, 0, 0, 0}, {0, 3, 4, 0, 1, 0}},
        {}};

    std::vector<std::vector<std::vector<std::vector<int>>>>
        clueNewNopesCombinations{
            {},
            {},
            // 2
            {{{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{1}, {}, {}, {}, {}, {}},
             {{}, {}, {5}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}}},
            // 3
            {{{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{4}, {3}, {}, {}, {}, {}},
             {{}, {1}, {}, {}, {}, {}},
             {{}, {3, 4}, {}, {}, {}, {}},
             {{}, {3}, {}, {}, {}, {}},
             {{1}, {}, {4}, {}, {}, {}},
             {{1}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{4}, {3}, {}, {}, {}, {}},
             {{2}, {}, {}, {}, {}, {}},
             {{}, {}, {3}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{}, {2}, {}, {}, {}, {}},
             {{}, {}, {}, {4}, {}, {}},
             {{3}, {}, {}, {}, {}, {}},
             {{1}, {4}, {}, {}, {}, {}}},
            // 4
            {{{}, {}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}},
             {{}, {}, {3}, {}, {}, {}},
             {{}, {3}, {}, {}, {}, {}},
             {{}, {}, {}, {}, {}, {}}},
            // 5
            {{{}, {1}, {1, 2}, {1, 2, 3}, {}, {}}, {{}, {}, {}, {}, {}, {}}},
            {},
        };

    for (std::size_t i = 0; i < clueFieldsStateCombinations[mClue].size();
         ++i) {
        if (!hasExpectedSkyscrapers(
                clueFieldsStateCombinations[mClue][i].skyscrapers)) {
            continue;
        }
        if (!hasExpectedNopes(clueFieldsStateCombinations[mClue][i].nopes)) {
            continue;
        }
        addNewSkyscrapers(clueNewSkyscraperCombinations[mClue][i]);
        addNewNopes(clueNewNopesCombinations[mClue][i]);
        // mClue = 0; // after we solved a field with a clue it is useless to
        // try again.
    }
}

void Slice::solveFromClueAndOpositeSliceClue()
{
    assert(mOppositeSlice);

    FieldsStates fieldsStateCombination;
    std::vector<int> newSkyscraperCombination;
    std::vector<std::vector<int>> newNopesCombinations;

    if (mClue == 1 && mOppositeSlice->clue() == 4) {
        fieldsStateCombination = {{6, 5, 0, 0, 0, 2}, {{}, {}, {}, {}, {}, {}}};
        newSkyscraperCombination = {{0, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{}, {}, {4}, {}, {3}, {}};
    }
    else if (mClue == 1 && mOppositeSlice->clue() == 5) {
        fieldsStateCombination = {{6, 5, 0, 4, 0, 0}, {{}, {}, {}, {}, {}, {}}};
        newSkyscraperCombination = {{0, 0, 0, 0, 0, 1}};
        newNopesCombinations = {{}, {}, {}, {}, {}, {}};
    }
    else if (mClue == 2 && mOppositeSlice->clue() == 1) {
        fieldsStateCombination = {{0, 0, 0, 0, 0, 6},
                                  {{}, {}, {}, {4, 5}, {3, 4, 5}, {}}};
        newSkyscraperCombination = {{5, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{}, {}, {}, {}, {}, {}};
    }
    else if (mClue == 2 && mOppositeSlice->clue() == 2) {
        fieldsStateCombination = {{0, 0, 0, 0, 0, 5},
                                  {{1, 4, 5}, {6}, {}, {}, {1, 2, 6}, {}}};
        newSkyscraperCombination = {{0, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{}, {3, 4}, {3, 4}, {}, {}, {}};
    }
    else if (mClue == 2 && mOppositeSlice->clue() == 3) {
        fieldsStateCombination = {{1, 6, 0, 0, 0, 0},
                                  {{}, {}, {}, {}, {5}, {5}}};
        newSkyscraperCombination = {{0, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{}, {}, {}, {4}, {}, {2}};
    }
    else if (mClue == 2 && mOppositeSlice->clue() == 4) {
        fieldsStateCombination = {{0, 0, 0, 5, 0, 0},
                                  {{}, {}, {}, {}, {}, {1}}};
        newSkyscraperCombination = {{0, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{}, {2, 3, 4}, {}, {}, {1, 2}, {}};
    }
    else if (mClue == 3 && mOppositeSlice->clue() == 2) {
        fieldsStateCombination = {{0, 0, 0, 0, 0, 0},
                                  {{1, 4}, {5}, {}, {}, {}, {5}}};
        newSkyscraperCombination = {{0, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{}, {3, 4}, {3, 4, 6}, {}, {}, {2, 3}};
    }
    else if (mClue == 3 && mOppositeSlice->clue() == 3) {
        fieldsStateCombination = {{0, 0, 0, 6, 5, 0},
                                  {{4}, {}, {}, {}, {}, {1, 2}}};

        newSkyscraperCombination = {{0, 0, 0, 0, 0, 0}};
        newNopesCombinations = {{3}, {2}, {2}, {}, {}, {}};
    }
    else {
        return;
    }

    if (!hasExpectedSkyscrapers(fieldsStateCombination.skyscrapers)) {
        return;
    }
    if (!hasExpectedNopes(fieldsStateCombination.nopes)) {
        return;
    }
    addNewSkyscrapers(newSkyscraperCombination);
    addNewNopes(newNopesCombinations);
}

void Slice::solveFromNopes()
{
    for (std::size_t i = 0; i < size(); ++i) {
        solveFromNopes(i + 1);
    }
}

void Slice::solveFromNopes(int searchSkyscraper)
{
    int idxWithNoSkyscraper = -1;

    for (std::size_t i = 0; i < size(); ++i) {
        if (mSkyscraperAtPos[i]) {
            if (skyscraper(i) == searchSkyscraper) {
                return;
            }
            continue;
        }
        else if (nopes(i).contains(searchSkyscraper)) {
            continue;
        }
        else { // existing node does not contain skyscaper
            if (idxWithNoSkyscraper != -1) {
                return;
            }
            idxWithNoSkyscraper = i;
        }
    }
    if (idxWithNoSkyscraper != -1) {
        insertSkyscraper(searchSkyscraper, idxWithNoSkyscraper);
    }
}

bool Slice::hasExpectedSkyscrapers(const std::vector<int> &expectedSkyscrapers)
{
    for (std::size_t i = 0; i < size(); ++i) {
        if (expectedSkyscrapers[i] == 0) {
            continue;
        }
        if (skyscraper(i) != expectedSkyscrapers[i]) {
            return false;
        }
    }
    return true;
}

bool Slice::hasExpectedNopes(const std::vector<std::vector<int>> &expectedNopes)
{
    for (std::size_t i = 0; i < size(); ++i) {
        if (expectedNopes[i].empty()) {
            continue;
        }
        if (mSkyscraperAtPos[i]) {
            return false;
        }
        if (!nopes(i).contains(expectedNopes[i])) {
            return false;
        }
    }
    return true;
}

void Slice::addNewSkyscrapers(const std::vector<int> &newSkyscrapers)
{
    for (std::size_t i = 0; i < size(); ++i) {
        if (newSkyscrapers[i] == 0) {
            continue;
        }
        insertSkyscraper(newSkyscrapers[i], i);
    }
}

void Slice::addNewNopes(const std::vector<std::vector<int>> &newNopes)
{
    for (std::size_t i = 0; i < newNopes.size(); ++i) {
        if (newNopes[i].empty()) {
            continue;
        }
        insertNopes(newNopes[i], i);
    }
}

void Slice::transformNopesToSkyscraperIfSizeReached(const Point &p, int idx)
{
    if (!mBoard.nopes[p.y][p.x].sizeReached()) {
        return;
    }
    int missingSkyscraper = mBoard.nopes[p.y][p.x].missingNumberInSequence();
    insertSkyscraper(missingSkyscraper, idx);
}

void Slice::insertLastMissingSkyscraper()
{
    int missingValueIdx = -1;
    std::vector<int> sequence;
    sequence.reserve(size() - 1);
    for (std::size_t i = 0; i < size(); ++i) {
        if (skyscraper(i) != 0) {
            sequence.emplace_back(skyscraper(i));
        }
        else {
            if (missingValueIdx != -1) {
                return;
            }
            missingValueIdx = i;
        }
    }

    if (missingValueIdx != -1) {
        int n = sequence.size() + 1;
        int projectedSum = (n + 1) * (n / 2);
        int actualSum = std::accumulate(sequence.begin(), sequence.end(), 0);
        int missingValue = projectedSum - actualSum;
        insertSkyscraper(missingValue, missingValueIdx);
    }
}

void Slice::addMissingNopesFromSkyscrapers()
{
    std::vector<int> exisitingSkyscrapers;
    exisitingSkyscrapers.reserve(size());

    bool foundNope = false;
    for (std::size_t i = 0; i < size(); ++i) {
        if (skyscraper(i) != 0) {
            exisitingSkyscrapers.emplace_back(skyscraper(i));
        }
        else if (!foundNope) {
            foundNope = true;
        }
    }
    if (!foundNope) {
        return;
    }
    if (exisitingSkyscrapers.empty()) {
        return;
    }
    for (std::size_t i = 0; i < size(); ++i) {
        if (skyscraper(i) == 0) {
            for (const auto &existingSkyscraper : exisitingSkyscrapers) {
                insertNopes(existingSkyscraper, i);
                if (skyscraper(i) != 0) {
                    break;
                }
            }
        }
    }
}

Point Slice::calcPosition(int idx) const
{
    Point point = mStartPoint;
    if (idx == 0) {
        return mStartPoint;
    }
    switch (mDirection) {
    case BorderPosition::top:
        point.y += idx;
        break;
    case BorderPosition::right:
        point.x -= idx;
        break;
    case BorderPosition::bottom:
        point.y -= idx;
        break;
    case BorderPosition::left:
        point.x += idx;
        break;
    }
    return point;
}

void debug_print(Board &board, const std::string &title = "")
{
    std::cout << title << '\n';
    for (std::size_t y = 0; y < board.skyscrapers.size(); ++y) {
        for (std::size_t x = 0; x < board.skyscrapers[y].size(); ++x) {
            if (board.skyscrapers[y][x] != 0) {
                std::cout << "V" << board.skyscrapers[y][x] << '\t';
            }
            else if (board.skyscrapers[y][x] == 0 &&
                     !board.nopes[y][x].isEmpty()) {
                auto nopes_set = board.nopes[y][x].values();
                std::vector<int> nopes(nopes_set.begin(), nopes_set.end());
                std::sort(nopes.begin(), nopes.end());

                for (std::size_t i = 0; i < nopes.size(); ++i) {
                    std::cout << nopes[i];

                    if (i == nopes.size() - 1) {
                        std::cout << '\t';
                    }
                    else {
                        std::cout << ',';
                    }
                }
            }
            else {
                std::cout << '\t';
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

std::vector<Slice> makeSlicesWithClues(Board &board,
                                       const std::vector<int> &clues)
{
    BorderIterator borderIterator{board.skyscrapers.size()};

    std::vector<Slice> slices;
    slices.reserve(clues.size());

    for (std::size_t i = 0; i < clues.size(); ++i, ++borderIterator) {
        slices.emplace_back(Slice{board, borderIterator.point(),
                                  borderIterator.borderPosition(), clues[i]});
    }
    return slices;
}

void connectSlicesWithCrossingSlices(std::vector<Slice> &slices)
{
    std::size_t boardSize = slices.size() / 4;

    std::vector<int> targetSlicesIdx;
    targetSlicesIdx.reserve(boardSize);
    for (std::size_t i = 0; i < slices.size(); ++i) {

        if (i % boardSize == 0) {
            targetSlicesIdx.clear();
            for (std::size_t j = 0; j < boardSize; ++j) {
                std::size_t idx = i < 4 ? slices.size() - 1 - j : i - 1 - j;
                targetSlicesIdx.push_back(idx);
            }
        }

        for (const auto &targetSliceIdx : targetSlicesIdx) {
            slices[i].addCrossingSlice(&slices[targetSliceIdx]);
        }
    }
}

void connectSlicesWithOppositeSlices(std::vector<Slice> &slices)
{
    std::size_t size = slices.size();
    std::size_t boardSize = size / 4;

    std::size_t offset = boardSize * 3 - 1;
    for (std::size_t i = 0; i < boardSize; ++i, offset -= 2) {
        std::size_t oppositeIdx = i + offset;
        slices[i].addOpposideSlice(&slices[oppositeIdx]);
    }
    offset = boardSize * 3 - 1;
    for (std::size_t i = boardSize; i < boardSize * 2; ++i, offset -= 2) {
        std::size_t oppositeIdx = i + offset;
        slices[i].addOpposideSlice(&slices[oppositeIdx]);
    }
    offset = boardSize + 1;
    for (std::size_t i = boardSize * 2; i < boardSize * 3; ++i, offset += 2) {
        std::size_t oppositeIdx = i - offset;
        slices[i].addOpposideSlice(&slices[oppositeIdx]);
    }
    offset = boardSize + 1;
    for (std::size_t i = boardSize * 3; i < slices.size(); ++i, offset += 2) {
        std::size_t oppositeIdx = i - offset;
        slices[i].addOpposideSlice(&slices[oppositeIdx]);
    }
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    constexpr int boardSize = 6;
    assert(clues.size() == boardSize * 4);
    Board board{boardSize};
    std::vector<Slice> slices = makeSlicesWithClues(board, clues);
    connectSlicesWithCrossingSlices(slices);
    connectSlicesWithOppositeSlices(slices);

    for (auto &slice : slices) {
        slice.addNopesFromClue();
    }

    int loopCount = 0;
    for (;;) {

        //        if (loopCount > 10000) {
        //            break;
        //        }

        bool allFullFirst = true;
        for (std::size_t i = 0; i < slices.size() / 2; ++i) {
            if (slices[i].isFull()) {
                continue;
            }
            ++loopCount;

            slices[i].solveFromNopes();
            slices[i].solveFromClueAndOpositeSliceClue();
            slices[i].solveFromClue();

            if (!slices[i].isFull()) {
                allFullFirst = false;
            }
        }

        if (allFullFirst) {
            break;
        }

        bool allFullSecond = true;
        for (std::size_t i = slices.size() / 2; i < slices.size(); ++i) {
            if (slices[i].isFull()) {
                continue;
            }
            ++loopCount;
            slices[i].solveFromClueAndOpositeSliceClue();
            slices[i].solveFromClue();

            if (!slices[i].isFull()) {
                allFullSecond = false;
            }
        }

        if (allFullSecond) {
            break;
        }
    }

    // debug_print(board);

    std::cout << "loop count: " << loopCount << '\n';
    return board.skyscrapers;
}
