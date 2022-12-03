#pragma once

#include <QVector>
#include "Cell.h"

namespace ColorLines
{

class GameField
{
public:
    GameField(const int cellsNb);

    int getCellsNb() const;

    QByteArray getState() const;
    void setState(const int cellsNb, const QByteArray& fieldState);

    const Cell &getCell(const int cellIdx) const;
    void setCellState(const int cellIdx, const QByteArray& state);

    void reset();

    QVector<int> getEmptyCells() const;

private:
    void resize(const int newCellsNb);

private:
    QVector<Cell> gameField_;
};

}
