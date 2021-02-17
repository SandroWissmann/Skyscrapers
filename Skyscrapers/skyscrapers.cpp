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
#include <iomanip>
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

enum class ReadDirection { topToBottom, rightToLeft };

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

void debug_print(Board &board, const std::string &title = "")
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
    int *mSkyscraper;
    Nopes *mNopes;
    bool mHasSkyscraper = false;
};

Field::Field(int &skyscraper, Nopes &nopes)
    : mSkyscraper{&skyscraper}, mNopes{&nopes}
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

bool Field::hasSkyscraper() const
{
    return mHasSkyscraper;
}

std::optional<int> Field::lastMissingNope() const
{
    if (!mNopes->sizeReached()) {
        return {};
    }
    return mNopes->missingNumberInSequence();
}

Point calcPosition(std::size_t idx, const Point &startPoint,
                   const ReadDirection &readDirection)
{
    Point point = startPoint;
    if (idx == 0) {
        return startPoint;
    }
    switch (readDirection) {
    case ReadDirection::topToBottom:
        point.y += idx;
        break;
    case ReadDirection::rightToLeft:
        point.x -= idx;
        break;
    }
    return point;
}

std::vector<std::vector<Field>> makeFields(Board &board)
{
    std::vector<std::vector<Field>> fields(board.skyscrapers.size());
    for (auto &row : fields) {
        row.reserve(fields.size());
    }
    for (std::size_t y = 0; y < board.skyscrapers.size(); ++y) {
        for (std::size_t x = 0; x < board.skyscrapers[y].size(); ++x) {
            fields[y].emplace_back(
                Field{board.skyscrapers[y][x], board.nopes[y][x]});
        }
    }
    return fields;
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

    bool isValidPermutation(const std::vector<int> &permutation) const;

    enum class Direction { front, back };

    bool hasSkyscrapers(const std::vector<int> &skyscrapers,
                        Direction direction) const;
    bool hasNopes(const std::vector<std::vector<int>> &nopes,
                  Direction direction) const;

    void addSkyscrapers(const std::vector<int> &skyscrapers,
                        Direction direction);
    void addNopes(const std::vector<std::vector<int>> &nopes,
                  Direction direction);

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

bool Row::isValidPermutation(const std::vector<int> &permutation) const
{
    assert(permutation.size() == size());

    auto permIt = permutation.cbegin();
    for (auto fieldIt = mRowFields.cbegin();
         fieldIt != mRowFields.cend() && permIt != permutation.end();
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

    std::vector<std::vector<int>> mPossiblePermutations;
    Row *mRow;
};

Slice::Slice(std::vector<std::vector<int>> &&possiblePermutations, Row &row)
    : mPossiblePermutations{possiblePermutations}, mRow{&row}
{
    if (mPossiblePermutations.empty()) {
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
    if (mPossiblePermutations.empty()) {
        return;
    }

    bool first = true;
    auto beforePerm = mPossiblePermutations.size();

    while (reducePossiblePermutations()) {

        if (first) {
            first = false;
            std::cout << this << "\tbefore:\t" << beforePerm << '\n';
        }

        std::cout << this << "\tafter:\t" << mPossiblePermutations.size()
                  << '\n';

        auto possibleBuildings = getPossibleBuildings();
        auto fieldElements = getFieldElements(possibleBuildings);

        mRow->addSkyscrapers(fieldElements.skyscrapers, Row::Direction::front);
        mRow->addNopes(fieldElements.nopes, Row::Direction::front);
    }
    if (!first) {
        std::cout << '\n';
    }
}

bool Slice::reducePossiblePermutations()
{
    auto startSize = mPossiblePermutations.size();
    auto it = mPossiblePermutations.begin();

    while (it != mPossiblePermutations.end()) {

        if (!mRow->isValidPermutation(*it)) {
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

std::vector<Row> makeRows(std::vector<std::vector<Field>> &fields)
{
    BorderIterator borderIterator{fields.size()};

    std::size_t size = fields.size() * 2;
    std::vector<Row> rows;
    rows.reserve(size);

    for (std::size_t i = 0; i < size; ++i, ++borderIterator) {
        rows.emplace_back(Row{fields, borderIterator.point(),
                              borderIterator.readDirection()});
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

void insertExistingSkyscrapersFromStartingGrid(
    std::vector<Row> &rows, const std::vector<std::vector<int>> startingGrid)
{
    assert(startingGrid.size() == rows.size() / 2);
    assert(startingGrid[0].size() == rows.size() / 2);

    std::size_t verticalRowsSize = rows.size() / 2;
    for (std::size_t x = 0; x < verticalRowsSize; ++x) {
        for (std::size_t y = 0; y < verticalRowsSize; ++y) {
            if (startingGrid[y][x] == 0) {
                continue;
            }
            rows[x].insertSkyscraper(static_cast<int>(y), startingGrid[y][x]);
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

std::vector<std::vector<int>>
SolvePuzzle(const std::vector<int> &clues,
            std::vector<std::vector<int>> startingGrid, int)
{
    assert(clues.size() % 4 == 0);

    auto cluePairs = makeCluePairs(clues);

    int boardSize = clues.size() / 4;
    Board board{boardSize};

    auto permutations = generatePermutations(boardSize);
    auto possiblePermutations =
        getPossiblePermutations(permutations, cluePairs);

    auto fields = makeFields(board);
    auto rows = makeRows(fields);
    connectRowsWithCrossingRows(rows);

    if (!startingGrid.empty()) {
        insertExistingSkyscrapersFromStartingGrid(rows, startingGrid);
    }

    std::vector<Slice> slices = makeSlices(possiblePermutations, rows);

    // debug_print(board);

    int count = 0;
    for (;;) {
        ++count;

        if (count > 1000) {
            debug_print(board);
            break;
        }

        bool allFullFirst = true;
        for (std::size_t i = 0; i < slices.size(); ++i) {
            if (slices[i].isSolved()) {
                continue;
            }
            slices[i].solveFromPossiblePermutations();
            slices[i].guessSkyscraperOutOfNeighbourNopes();

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

std::vector<std::vector<int>> SolvePuzzle(const std::vector<int> &clues)
{
    return SolvePuzzle(clues, std::vector<std::vector<int>>{}, 0);
}
