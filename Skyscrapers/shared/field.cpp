#include "field.h"

#include "../shared/nopes.h"

#include <cassert>

Field::Field(std::size_t rowSize)
    : mSkyscraper{0}, mNopes{static_cast<int>(rowSize)}
{
}

void Field::insertSkyscraper(int skyscraper)
{
    //    assert(mSkyscraper == 0 || skyscraper == mSkyscraper);
    //    if (hasSkyscraper()) {
    //        return;
    //    }
    mSkyscraper = skyscraper;
    mNopes.clear();
}
void Field::insertNope(int nope)
{
    if (hasSkyscraper()) {
        return;
    }
    mNopes.insert(nope);
}
void Field::insertNopes(const std::vector<int> &nopes)
{
    if (hasSkyscraper()) {
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
    return mSkyscraper != 0;
}

std::optional<int> Field::lastMissingNope() const
{
    if (!mNopes.sizeReached()) {
        return {};
    }
    return mNopes.missingNumberInSequence();
}
