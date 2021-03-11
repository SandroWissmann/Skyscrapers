#include "field.h"

#include "../shared/nopes.h"

#include <cassert>

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
