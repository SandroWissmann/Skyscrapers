#include "row.h"

#include "borderiterator.h"
#include "field.h"
#include "nopes.h"
#include "point.h"

#include <algorithm>
#include <cassert>
#include <unordered_map>

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
