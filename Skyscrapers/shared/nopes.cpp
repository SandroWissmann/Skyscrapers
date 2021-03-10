#include "nopes.h"

#include <cassert>

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
