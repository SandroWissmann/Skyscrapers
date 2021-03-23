#include "field.h"

#include <cassert>

void Field::insertSkyscraper(int skyscraper)
{
    mBitmask = 1 << (skyscraper - 1);
}

void Field::insertNope(int nope)
{
    mBitmask &= ~(1 << (nope - 1));
}

void Field::insertNopes(const std::vector<int> &nopes)
{
    for (const auto nope : nopes) {
        insertNope(nope);
    }
}

int Field::skyscraper(std::size_t size) const
{
    if (!hasSkyscraper()) {
        return 0;
    }

    for (std::size_t i = 0; i < size; ++i) {
        if (bitIsToggled(mBitmask, i)) {
            return i + 1;
        }
    }
    assert(false);
    return 0;
}

std::vector<int> Field::nopes(std::size_t size) const
{
    std::vector<int> nopes;
    nopes.reserve(size - 1);
    for (std::size_t i = 0; i < size; ++i) {
        if (!bitIsToggled(mBitmask, i)) {
            nopes.emplace_back(i + 1);
        }
    }
    return nopes;
}

bool Field::hasSkyscraper() const
{
    return hasSingleBit(bitmask());
}

bool Field::containsNope(int value) const
{
    return !bitIsToggled(mBitmask, value - 1);
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

bool Field::hasSingleBit(BitmaskType bitmask) const
{
    return bitmask != 0 && (bitmask & (bitmask - 1)) == 0;
}
