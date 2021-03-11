#ifndef FIELD_H
#define FIELD_H

#include <optional>
#include <vector>

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
    int &mSkyscraper;
    Nopes &mNopes;
    bool mHasSkyscraper = false;
};

#endif
