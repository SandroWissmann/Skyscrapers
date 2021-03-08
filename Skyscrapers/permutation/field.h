#ifndef PERMUTATION_FIELD_H
#define PERMUTATION_FIELD_H

#include <optional>
#include <vector>

namespace permutation {

class Nopes;

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

std::vector<std::vector<Field>>
makeFields(std::vector<std::vector<int>> &skyscrapers,
           std::vector<std::vector<Nopes>> &nopes);

} // namespace permutation

#endif
