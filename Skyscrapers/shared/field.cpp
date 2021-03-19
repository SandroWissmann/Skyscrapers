#include "field.h"

#include <cassert>

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
