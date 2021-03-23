#ifndef ROW_H
#define ROW_H

#include "../shared/readdirection.h"

#include <optional>
#include <vector>

class Field;
class Point;

class Row {
public:
    Row(std::vector<Field> &fields, std::size_t size, const Point &startPoint,
        const ReadDirection &readDirection);

    std::size_t size() const;

    void addCrossingRows(Row *crossingRow);

    bool hasOnlyOneNopeField() const;
    void addLastMissingSkyscraper();

    void addNopesToAllNopeFields(int nope);

    bool allFieldsContainSkyscraper() const;

    int skyscraperCount() const;
    int nopeCount(int nope) const;

    void guessSkyscraperOutOfNeighbourNopes();

    enum class Direction { front, back };

    bool hasSkyscrapers(const std::vector<int> &skyscrapers,
                        Direction direction) const;
    bool hasNopes(const std::vector<std::vector<int>> &nopes,
                  Direction direction) const;

    void addFieldData(const std::vector<Field> &fieldData, Direction direction);

    void addSkyscrapers(const std::vector<int> &skyscrapers,
                        Direction direction);
    void addNopes(const std::vector<std::vector<int>> &nopes,
                  Direction direction);

    std::vector<Field *> getFields() const;

private:
    template <typename SkyIterator, typename FieldIterator>
    bool hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                        FieldIterator fieldItBegin,
                        FieldIterator fieldItEnd) const;

    template <typename NopesIterator, typename FieldIterator>
    bool hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                  FieldIterator fieldItBegin, FieldIterator fieldItEnd) const;

    template <typename FieldDataIterator, typename FieldIterator>
    void addFieldData(FieldDataIterator fieldDataItBegin,
                      FieldDataIterator fieldDataItEnd,
                      FieldIterator fieldItBegin, FieldIterator fieldItEnd);

    template <typename SkyIterator, typename FieldIterator>
    void addSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd,
                        FieldIterator fieldItBegin, FieldIterator fieldItEnd);

    template <typename NopesIterator, typename FieldIterator>
    void addNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd,
                  FieldIterator fieldItBegin, FieldIterator fieldItEnd);

    template <typename FieldIterator>
    void insertFieldData(const FieldIterator &fieldIt, const Field &fieldData);

    template <typename FieldIterator>
    void insertSkyscraper(FieldIterator fieldIt, int skyscraper);

    template <typename FieldIterator>
    void insertSkyscraperNeighbourHandling(FieldIterator fieldIt,
                                           int skyscraper);

    template <typename FieldIterator>
    void insertNope(FieldIterator fieldIt, int nope);

    template <typename FieldIterator>
    void insertNopesNeighbourHandling(FieldIterator fieldIt, int nopes,
                                      bool hadSkyscraperBefore);

    template <typename FieldIterator>
    void insertNopes(FieldIterator fieldIt, const std::vector<int> &nopes);

    int getIdx(std::vector<Field *>::const_iterator cit) const;
    int getIdx(std::vector<Field *>::const_reverse_iterator crit) const;

    std::vector<Field *> getRowFields(const ReadDirection &readDirection,
                                      std::vector<Field> &boardFields,
                                      std::size_t size,
                                      const Point &startPoint);

    bool onlyOneFieldWithoutNope(int nope) const;

    bool nopeExistsAsSkyscraperInFields(const std::vector<Field *> &rowFields,
                                        int nope) const;

    std::optional<int> nopeValueInAllButOneField() const;

    void insertSkyscraperToFirstFieldWithoutNope(int nope);

    bool hasSkyscraper(int skyscraper) const;

    std::vector<Row *> mCrossingRows;
    std::vector<Field *> mRowFields;
};

#endif
