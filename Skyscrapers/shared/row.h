#ifndef ROW_H
#define ROW_H

#include "../shared/point.h"
#include "../shared/readdirection.h"

#include <optional>
#include <vector>

class Field;
class Board;

class Row {
public:
    Row(Board &board, const Point &startPoint,
        const ReadDirection &readDirection);

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

    const Field &getFieldRef(std::size_t idx) const;
    Field &getFieldRef(std::size_t idx);

private:
    template <typename SkyIterator>
    bool hasSkyscrapers(SkyIterator skyItBegin, SkyIterator skyItEnd) const;

    template <typename NopesIterator>
    bool hasNopes(NopesIterator nopesItBegin, NopesIterator nopesItEnd) const;

    template <typename FieldDataIterator>
    void addFieldData(FieldDataIterator fieldDataItBegin,
                      FieldDataIterator fieldDataItEnd);

    void insertFieldData(std::size_t idx, const Field &fieldData);

    void insertSkyscraperNeighbourHandling(std::size_t idx, int skyscraper);

    void insertNopesNeighbourHandling(std::size_t idx, int nopes,
                                      bool hadSkyscraperBefore);

    bool onlyOneFieldWithoutNope(int nope) const;

    bool nopeExistsAsSkyscraperInFields(int nope) const;

    std::optional<int> nopeValueInAllButOneField() const;

    void insertSkyscraperToFirstFieldWithoutNope(int nope);

    bool hasSkyscraper(int skyscraper) const;

    // Field &getFieldRef(std::size_t idx);

    Board &mBoard;
    Point mStartPoint;
    ReadDirection mReadDirection;
    std::vector<Row *> mCrossingRows;
};

#endif
