#ifndef FIELD_H
#define FIELD_H

#include "nopes.h"

#include <optional>
#include <vector>

class Field {
public:
    Field(std::size_t rowSize);

    void insertSkyscraper(int skyscraper);
    void insertNope(int nope);
    void insertNopes(const std::vector<int> &nopes);

    bool fullOfNopes() const;

    int skyscraper() const;
    Nopes nopes() const;

    bool hasSkyscraper() const;

    std::optional<int> lastMissingNope() const;

private:
    int mSkyscraper;
    Nopes mNopes;
};

#endif
