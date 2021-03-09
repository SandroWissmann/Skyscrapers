#include "field.h"

#include "nopes.h"

#include <cassert>

namespace permutation {

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

// not used
std::vector<std::vector<Field>>
makeFields(std::vector<std::vector<int>> &skyscrapers,
           std::vector<std::vector<Nopes>> &nopes)
{
    std::vector<std::vector<Field>> fields(skyscrapers.size());
    for (auto &row : fields) {
        row.reserve(fields.size());
    }
    for (std::size_t y = 0; y < skyscrapers.size(); ++y) {
        fields[y].reserve(skyscrapers.size());
        for (std::size_t x = 0; x < skyscrapers[y].size(); ++x) {
            fields[y].emplace_back(Field{skyscrapers[y][x], nopes[y][x]});
        }
    }
    return fields;
}

} // namespace permutation
