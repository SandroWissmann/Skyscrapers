#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <unordered_set>

#include <chrono>
#include <iostream>

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
    return ::missingNumberInSequence(mValues.begin(), mValues.end());
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

enum class BorderPosition { top, right };

void nextDirection(BorderPosition &borderPosition)
{
    assert(borderPosition != BorderPosition::right);
    int dir = static_cast<int>(borderPosition);
    ++dir;
    borderPosition = static_cast<BorderPosition>(dir);
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

Board board{4};

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

struct Field {
public:
    Field(Board &board, const Point &point);
    Field(int &skyscraper, Nopes &nopes);

    void insertSkyscraper(int skyscraper);
    void insertNope(int nope);
    void insertNopes(const std::vector<int> &nopes);

    bool fullOfNopes() const;

    int skyscraper() const;
    Nopes nopes() const;

    Point point() const;
    bool hasSkyscraper() const;
    void markHasSkyscraper();

    void transformToSkyscraper();

private:
    Board *mBoard;
    Point mPoint;
    int *mSkyscraper;
    Nopes *mNopes;
    bool mHasSkyscraper = false;
};

Field::Field(Board &board, const Point &point)
    : mBoard{&board}, mPoint{point},
      mSkyscraper{&mBoard->skyscrapers[mPoint.y][mPoint.x]},
      mNopes{&mBoard->nopes[mPoint.y][mPoint.x]}
{
}

void Field::insertSkyscraper(int skyscraper)
{
    assert(*mSkyscraper == 0 || skyscraper == *mSkyscraper);
    if (mHasSkyscraper) {
        return;
    }
    *mSkyscraper = skyscraper;
    mHasSkyscraper = true;
    // potentially performance problem ???
    mNopes->clear();
}
void Field::insertNope(int nope)
{
    if (mHasSkyscraper) {
        return;
    }
    mNopes->insert(nope);
}
void Field::insertNopes(const std::vector<int> &nopes)
{
    if (mHasSkyscraper) {
        return;
    }
    mNopes->insert(nopes);
}

bool Field::fullOfNopes() const
{
    return mNopes->sizeReached();
}

int Field::skyscraper() const
{
    return *mSkyscraper;
}
Nopes Field::nopes() const
{
    return *mNopes;
}

Point Field::point() const
{
    return mPoint;
}

bool Field::hasSkyscraper() const
{
    return mHasSkyscraper;
}

void Field::markHasSkyscraper()
{
    mHasSkyscraper = true;
}

void Field::transformToSkyscraper()
{
    if (!mNopes->sizeReached()) {
        return;
    }
    auto missingSkyscraper = mNopes->missingNumberInSequence();
    insertSkyscraper(missingSkyscraper);
}

Point calcPosition(std::size_t idx, const Point &startPoint,
                   const BorderPosition &borderPosition)
{
    Point point = startPoint;
    if (idx == 0) {
        return startPoint;
    }
    switch (borderPosition) {
    case BorderPosition::top:
        point.y += idx;
        break;
    case BorderPosition::right:
        point.x -= idx;
        break;
    }
    return point;
}

std::vector<std::vector<Field>> makeFields(Board &board)
{
    std::vector<std::vector<Field>> fields;
    fields.reserve(board.skyscrapers.size() * board.skyscrapers[0].size());

    for (std::size_t y = 0; y < board.skyscrapers.size(); ++y) {
        for (std::size_t x = 0; x < board.skyscrapers[y].size(); ++x) {
            fields.emplace_back(
                Field(board.skyscrapers[y][x], board.nopes[y][x]));
        }
    }
    return fields;
}

std::vector<Field> makeFields(Board &board, const Point &startPoint,
                              const BorderPosition &borderPosition)
{
    std::vector<Field> fields;
    fields.reserve(board.skyscrapers.size());
    for (std::size_t i = 0; i < board.skyscrapers.size(); ++i) {
        auto point = calcPosition(i, startPoint, borderPosition);
        fields.push_back(Field{board, point});
    }
    return fields;
}

class Row {
public:
    Row(const std::vector<Field> &fields);

    template <typename RawIteratorType> struct RawIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = RawIteratorType;
        using pointer = value_type *;
        using reference = value_type &;

        RawIterator(pointer ptr) : mPtr(ptr)
        {
        }

        reference operator*() const
        {
            return *mPtr;
        }
        pointer operator->()
        {
            return mPtr;
        }

        RawIterator &operator++()
        {
            mPtr++;
            return *this;
        }
        RawIterator operator++(int)
        {
            RawIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const RawIterator &a, const RawIterator &b)
        {
            return a.mPtr == b.mPtr;
        };
        friend bool operator!=(const RawIterator &a, const RawIterator &b)
        {
            return a.mPtr != b.mPtr;
        };

    private:
        pointer mPtr;
    };

    template <typename RawReverseIteratorType> struct RawReverseIterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = RawReverseIteratorType;
        using pointer = value_type *;
        using reference = value_type &;

        RawReverseIterator(pointer ptr) : mPtr(ptr)
        {
        }

        reference operator*() const
        {
            return *mPtr;
        }
        pointer operator->()
        {
            return mPtr;
        }

        RawReverseIterator &operator++()
        {
            mPtr--;
            return *this;
        }
        RawReverseIterator operator++(int)
        {
            RawReverseIterator tmp = *this;
            --mPtr;
            return tmp;
        }

        friend bool operator==(const RawReverseIterator &a,
                               const RawReverseIterator &b)
        {
            return a.mPtr == b.mPtr;
        };
        friend bool operator!=(const RawReverseIterator &a,
                               const RawReverseIterator &b)
        {
            return a.mPtr != b.mPtr;
        };

    private:
        pointer mPtr;
    };

    typedef RawIterator<Field> iterator;
    typedef RawIterator<const Field> const_iterator;

    typedef RawReverseIterator<Field> reverse_iterator;
    typedef RawReverseIterator<const Field> const_reverse_iterator;

    iterator begin()
    {
        return iterator(&mFields[0]);
    }
    iterator end()
    {
        return iterator(&mFields[mFields.size()]);
    }

    const_iterator cbegin() const
    {
        return const_iterator(&mFields[0]);
    }
    const_iterator cend() const
    {
        return const_iterator(&mFields[mFields.size()]);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(&mFields[mFields.size() - 1]);
    }
    reverse_iterator rend()
    {
        return reverse_iterator(&mFields[-1]);
    }

    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(&mFields[mFields.size() - 1]);
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(&mFields[-1]);
    }

    std::size_t size() const;

    void addCrossingRows(Row *crossingRow);

    template <typename IteratorType>
    void insertSkyscraper(IteratorType it, int skyscraper);

    template <typename IteratorType> void insertNope(IteratorType it, int nope);

    template <typename IteratorType>
    void insertNopes(IteratorType it, const std::vector<int> &nopes);

    iterator toIterator(const Point &point);

    template <typename IteratorType>
    void addMissingSkyscraperMark(IteratorType it);

    void addNopesToAllNopeFields(int nope);

    int getIdx(iterator it);
    int getIdx(reverse_iterator rit);

    bool hasOnlyOneNopeField() const;
    void addLastMissingSkyscraper();

    bool allFieldsContainSkyscraper() const;

    int skyscraperCount() const;
    int nopeCount(int nope) const;

    void guessSkyscraperOutOfNeighbourNopes();

private:
    bool onlyOneFieldWithoutNope(int nope) const;
    std::optional<int> nopeValueInAllButOneField() const;

    void insertSkyscraperToFirstFieldWithoutNope(int nope);

    bool hasSkyscraper(int skyscraper) const;

    std::vector<Row *> mCrossingRows;
    std::vector<Field> mFields;
};

Row::Row(const std::vector<Field> &fields) : mFields{fields}
{
}

std::size_t Row::size() const
{
    return mFields.size();
}

void Row::addCrossingRows(Row *crossingRow)
{
    assert(crossingRow != nullptr);
    assert(mCrossingRows.size() < mFields.size());
    mCrossingRows.push_back(crossingRow);
}

template <typename IteratorType>
void Row::insertSkyscraper(IteratorType it, int skyscraper)
{
    assert(mCrossingRows.size() == mFields.size());

    if (it->hasSkyscraper()) {
        return;
    }
    it->insertSkyscraper(skyscraper);
    if (hasOnlyOneNopeField()) {
        addLastMissingSkyscraper();
    }
    debug_print(board);
    addNopesToAllNopeFields(skyscraper);
    debug_print(board);

    int idx = getIdx(it);
    auto oppositeIt = mCrossingRows[idx]->toIterator(it->point());
    oppositeIt->markHasSkyscraper();
    if (mCrossingRows[idx]->hasOnlyOneNopeField()) {
        mCrossingRows[idx]->addLastMissingSkyscraper();
        debug_print(board);
    }

    mCrossingRows[idx]->addNopesToAllNopeFields(skyscraper);
    debug_print(board);
}

template <typename IteratorType> void Row::insertNope(IteratorType it, int nope)
{
    if (it->hasSkyscraper()) {
        return;
    }
    if (it->nopes().contains(nope)) {
        return;
    }
    it->insertNope(nope);

    it->transformToSkyscraper();
    if (onlyOneFieldWithoutNope(nope)) {
        insertSkyscraperToFirstFieldWithoutNope(nope);
    }

    int idx = getIdx(it);

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

Row::iterator Row::toIterator(const Point &point)
{
    for (auto it = begin(); it != end(); ++it) {
        if (it->point() == point) {
            return it;
        }
    }
    return end();
}

void Row::addNopesToAllNopeFields(int nope)
{
    for (auto it = begin(); it != end(); ++it) {
        if (it->hasSkyscraper()) {
            continue;
        }
        insertNope(it, nope);
    }
}

int Row::getIdx(Row::iterator it)
{
    return std::distance(begin(), it);
}

int Row::getIdx(Row::reverse_iterator rit)
{
    return mFields.size() - std::distance(rbegin(), rit) - 1;
}

bool Row::hasOnlyOneNopeField() const
{
    return skyscraperCount() == static_cast<int>(mFields.size() - 1);
}

void Row::addLastMissingSkyscraper()
{
    assert(hasOnlyOneNopeField());

    iterator nopeFieldIt = end();
    std::vector<int> sequence;
    sequence.reserve(mFields.size() - 1);

    for (auto it = begin(); it != end(); ++it) {
        if (it->hasSkyscraper()) {
            sequence.emplace_back(it->skyscraper());
        }
        else {
            nopeFieldIt = it;
        }
    }
    assert(nopeFieldIt != end());
    assert(skyscraperCount() == static_cast<int>(sequence.size()));
    auto missingValue =
        missingNumberInSequence(sequence.begin(), sequence.end());
    assert(missingValue >= 0 &&
           missingValue <= static_cast<int>(mFields.size()));
    insertSkyscraper(nopeFieldIt, missingValue);
}

bool Row::allFieldsContainSkyscraper() const
{
    return skyscraperCount() == static_cast<int>(mFields.size());
}

int Row::skyscraperCount() const
{
    int count = 0;
    for (auto cit = cbegin(); cit != cend(); ++cit) {
        if (cit->hasSkyscraper()) {
            ++count;
        }
    }
    return count;
}

int Row::nopeCount(int nope) const
{
    int count = 0;
    for (auto cit = cbegin(); cit != cend(); ++cit) {
        if (cit->nopes().contains(nope)) {
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

bool Row::onlyOneFieldWithoutNope(int nope) const
{
    auto cit = std::find_if(cbegin(), cend(), [nope](const Field &field) {
        return field.skyscraper() == nope;
    });
    if (cit != cend()) {
        return false;
    }
    if (nopeCount(nope) <
        static_cast<int>(mFields.size()) - skyscraperCount() - 1) {
        return false;
    }
    return true;
}

std::optional<int> Row::nopeValueInAllButOneField() const
{
    std::unordered_map<int, int> nopeAndCount;

    for (auto cit = cbegin(); cit != cend(); ++cit) {
        if (!cit->hasSkyscraper()) {
            auto nopes = cit->nopes().containing();
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
            static_cast<int>(mFields.size()) - skyscraperCount() - 1) {
            return {cit->first};
        }
    }
    return {};
}

void Row::insertSkyscraperToFirstFieldWithoutNope(int nope)
{
    for (auto it = begin(); it != end(); ++it) {
        if (it->hasSkyscraper()) {
            continue;
        }
        if (!it->nopes().contains(nope)) {
            insertSkyscraper(it, nope);
            return; // there can be max one skyscraper per row;
        }
    }
}

bool Row::hasSkyscraper(int skyscraper) const
{
    for (const auto &field : mFields) {
        if (field.skyscraper() == skyscraper) {
            return true;
        }
    }
    return false;
}

struct CluePair {
    int front{};
    int back{};
};

struct FieldElements {
    std::vector<int> skyscrapers{};
    std::vector<std::vector<int>> nopes{};
};

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

class Slice {
public:
    Slice(std::vector<std::vector<int>> &&possiblePermutations, Row &row);

    void guessSkyscraperOutOfNeighbourNopes();

    bool isSolved() const;

    void solveFromPossiblePermutations();

    bool reducePossiblePermutations();

private:
    std::vector<std::set<int>> getPossibleBuildings() const;

    FieldElements
    getFieldElements(const std::vector<std::set<int>> &possibleBuildings);

    bool isValidPermutation(const std::vector<int> &permutation) const;

    template <typename RowIterator>
    void addElementsFromCurrentElementsAndClue(int clue, RowIterator rowItBegin,
                                               RowIterator rowItEnd);

    template <typename SkyIterator, typename RowIterator>
    bool hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                        RowIterator rowItBegin, RowIterator rowItEnd);

    template <typename NopesIterator, typename RowIterator>
    bool hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                  RowIterator rowItBegin, RowIterator rowItEnd);

    template <typename SkyIterator, typename RowIterator>
    void addSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                        RowIterator rowItBegin, RowIterator rowItEnd);

    template <typename NopesIterator, typename RowIterator>
    void addNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                  RowIterator rowItBegin, RowIterator rowItEnd);

    std::vector<std::vector<int>> mPossiblePermutations;
    Row *mRow;
    const CluePair mCluePair;
};

Slice::Slice(std::vector<std::vector<int>> &&possiblePermutations, Row &row)
    : mPossiblePermutations{possiblePermutations}, mRow{&row}
{
    if (mPossiblePermutations.empty()) {
        return;
    }
    auto possibleBuildings = getPossibleBuildings();
    auto fieldElements = getFieldElements(possibleBuildings);
    addSkyscrapers(fieldElements.skyscrapers.cbegin(),
                   fieldElements.skyscrapers.cend(), mRow->begin(),
                   mRow->end());

    addNopes(fieldElements.nopes.cbegin(), fieldElements.nopes.cend(),
             mRow->begin(), mRow->end());
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
    if (mPossiblePermutations.empty()) {
        return;
    }
    while (reducePossiblePermutations()) {
        auto possibleBuildings = getPossibleBuildings();
        auto fieldElements = getFieldElements(possibleBuildings);
        addSkyscrapers(fieldElements.skyscrapers.cbegin(),
                       fieldElements.skyscrapers.cend(), mRow->begin(),
                       mRow->end());
        addNopes(fieldElements.nopes.cbegin(), fieldElements.nopes.cend(),
                 mRow->begin(), mRow->end());
    }
}

bool Slice::reducePossiblePermutations()
{
    auto startSize = mPossiblePermutations.size();
    auto it = mPossiblePermutations.begin();

    while (it != mPossiblePermutations.end()) {

        if (!isValidPermutation(*it)) {
            it = mPossiblePermutations.erase(it);
        }
        else {
            ++it;
        }
    }
    return startSize > mPossiblePermutations.size();
}

std::vector<std::set<int>> Slice::getPossibleBuildings() const
{
    std::vector<std::set<int>> possibleBuildingsOnFields(mRow->size());

    for (const auto &permutation : mPossiblePermutations) {
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

bool Slice::isValidPermutation(const std::vector<int> &permutation) const
{
    assert(permutation.size() == mRow->size());

    auto permIt = permutation.cbegin();
    for (auto rowIt = mRow->cbegin();
         rowIt != mRow->cend() && permIt != permutation.end();
         ++rowIt, ++permIt) {

        if (rowIt->hasSkyscraper()) {
            if (rowIt->skyscraper() != *permIt) {
                return false;
            }
        }
        else if (!rowIt->nopes().isEmpty()) {
            if (rowIt->nopes().contains(*permIt)) {
                return false;
            }
        }
    }
    return true;
}

template <typename SkyIterator, typename RowIterator>
bool Slice::hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                           RowIterator rowItBegin, RowIterator rowItEnd)
{
    auto skyIt = skyItBegin;
    for (auto rowIt = rowItBegin; rowIt != rowItEnd && skyIt != skyItEnd;
         ++rowIt, ++skyIt) {
        if (*skyIt == 0 && rowIt->hasSkyscraper()) {
            continue;
        }
        if (rowIt->skyscraper() != *skyIt) {
            return false;
        }
    }
    return true;
}

template <typename NopesIterator, typename RowIterator>
bool Slice::hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                     RowIterator rowItBegin, RowIterator rowItEnd)
{
    auto nopesIt = nopesItBegin;
    for (auto rowIt = rowItBegin; rowIt != rowItEnd && nopesIt != nopesItEnd;
         ++rowIt, ++nopesIt) {

        if (nopesIt->empty()) {
            continue;
        }
        if (rowIt->hasSkyscraper()) {
            return false;
        }
        if (!rowIt->nopes().contains(*nopesIt)) {
            return false;
        }
    }
    return true;
}

template <typename SkyIterator, typename RowIterator>
void Slice::addSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                           RowIterator rowItBegin, RowIterator rowItEnd)
{
    auto skyIt = skyItBegin;
    for (auto rowIt = rowItBegin; rowIt != rowItEnd && skyIt != skyItEnd;
         ++rowIt, ++skyIt) {
        if (*skyIt == 0) {
            continue;
        }
        mRow->insertSkyscraper(rowIt, *skyIt);
    }
}

template <typename NopesIterator, typename RowIterator>
void Slice::addNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                     RowIterator rowItBegin, RowIterator rowItEnd)
{
    auto nopesIt = nopesItBegin;
    for (auto rowIt = rowItBegin; rowIt != rowItEnd && nopesIt != nopesItEnd;
         ++rowIt, ++nopesIt) {
        if (nopesIt->empty()) {
            continue;
        }
        mRow->insertNopes(rowIt, *nopesIt);
    }
}

std::vector<Row> makeRows(Board &board)
{
    BorderIterator borderIterator{board.skyscrapers.size()};

    std::size_t size = board.skyscrapers.size() * 2;
    std::vector<Row> rows;
    rows.reserve(size);
    for (std::size_t i = 0; i < size; ++i, ++borderIterator) {
        auto fields = makeFields(board, borderIterator.point(),
                                 borderIterator.borderPosition());

        rows.emplace_back(Row{fields});
    }
    return rows;
}

void connectRowsWithCrossingRows(std::vector<Row> &rows)
{
    std::size_t boardSize = rows.size() / 2;

    std::vector<int> targetRowsIdx(boardSize);
    std::iota(targetRowsIdx.begin(), targetRowsIdx.end(), boardSize);

    for (std::size_t i = 0; i < rows.size(); ++i) {
        if (i == rows.size() / 2) {
            std::iota(targetRowsIdx.begin(), targetRowsIdx.end(), 0);
            std::reverse(targetRowsIdx.begin(), targetRowsIdx.end());
        }

        for (const auto &targetRowIdx : targetRowsIdx) {
            rows[i].addCrossingRows(&rows[targetRowIdx]);
        }
    }
}

std::vector<Slice>
makeSlices(std::vector<std::vector<std::vector<int>>> &possiblePermutations,
           std::vector<Row> &rows)
{
    std::vector<Slice> slices;
    slices.reserve(possiblePermutations.size());

    for (std::size_t i = 0; i < possiblePermutations.size(); ++i) {
        slices.emplace_back(Slice{std::move(possiblePermutations[i]), rows[i]});
    }

    return slices;
}

int factorial(int n)
{
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

std::vector<std::vector<int>> generatePermutations(int size)
{
    std::vector<int> sequence(size);
    std::iota(sequence.begin(), sequence.end(), 1);

    std::vector<std::vector<int>> permutations;
    permutations.reserve(factorial(sequence.size()));

    do {
        permutations.emplace_back(sequence);
    } while (std::next_permutation(sequence.begin(), sequence.end()));
    return permutations;
}

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

std::vector<std::vector<std::vector<int>>>
getPossiblePermutations(const std::vector<std::vector<int>> &permutations,
                        const std::vector<CluePair> &cluePairs)
{
    std::vector<std::vector<std::vector<int>>> result(cluePairs.size());
    for (auto &elem : result) {
        elem.reserve(permutations.size() / 3);
    }

    for (const auto &permutation : permutations) {

        auto front = buildingsVisible(permutation.cbegin(), permutation.cend());
        auto back =
            buildingsVisible(permutation.crbegin(), permutation.crend());

        for (std::size_t i = 0; i < cluePairs.size(); ++i) {
            if (cluePairs[i].front == 0 && cluePairs[i].back == 0) {
                continue;
            }
            if (cluePairs[i].front != 0 && cluePairs[i].front != front) {
                continue;
            }
            if (cluePairs[i].back != 0 && cluePairs[i].back != back) {
                continue;
            }
            result[i].emplace_back(permutation);
        }
    }
    return result;
}

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    assert(clues.size() % 4 == 0);

    auto cluePairs = makeCluePairs(clues);

    int boardSize = clues.size() / 4;
    // Board board{boardSize};

    auto permutations = generatePermutations(boardSize);
    auto possiblePermutations =
        getPossiblePermutations(permutations, cluePairs);

    auto rows = makeRows(board);
    connectRowsWithCrossingRows(rows);
    std::vector<Slice> slices = makeSlices(possiblePermutations, rows);

    debug_print(board);

    int count = 0;
    for (;;) {
        bool allFullFirst = true;
        for (std::size_t i = 0; i < slices.size(); ++i) {
            ++count;
            if (count > 1000) {
                debug_print(board);
                break;
            }

            if (slices[i].isSolved()) {
                continue;
            }

            if (i == 1) {
                i = 1;
            }

            std::cout << "i:\t" << i << '\n';
            debug_print(board);

            slices[i].solveFromPossiblePermutations();

            debug_print(board);

            slices[i].guessSkyscraperOutOfNeighbourNopes();

            debug_print(board);

            if (!slices[i].isSolved()) {
                allFullFirst = false;
            }
        }

        if (allFullFirst) {
            break;
        }
    }
    return board.skyscrapers;
}
