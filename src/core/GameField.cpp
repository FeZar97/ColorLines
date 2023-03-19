#include "GameField.h"

using namespace ColorLines;

GameField::GameField(const int cellsNb)
{
    resize(cellsNb);
}

int GameField::getCellsNb() const
{
    return gameField_.size();
}

QByteArray ColorLines::GameField::getState() const
{
    QByteArray state;
    state.reserve(gameField_.size() * sizeof(Cell));
    for (const Cell& cell: gameField_)
    {
        state.append(cell.getBytes());
    }

    return state;
}

void GameField::setState(const int cellsNb, const QByteArray &fieldState)
{
    Q_ASSERT(fieldState.size() == cellsNb * sizeof(Cell));

    resize(cellsNb);
    for (int cellIdx = 0; cellIdx < cellsNb; cellIdx++)
    {
        gameField_[cellIdx].setState(fieldState.mid(cellIdx * sizeof(Cell), sizeof(Cell)));
    }
}

const Cell &GameField::getCell(const int cellIdx) const
{
    return gameField_.at(cellIdx);
}

void GameField::setCellState(const int cellIdx, const QByteArray &state)
{
    Q_ASSERT(cellIdx >= 0 && cellIdx < gameField_.size());

    gameField_[cellIdx].setState(state);
}

void GameField::reset()
{
    for (int cellIdx = 0; cellIdx < gameField_.size(); cellIdx++)
    {
        gameField_[cellIdx].setState({sizeof(Cell), 0});
    }
}

QVector<int> GameField::getEmptyCells() const
{
    QVector<int> emptyCellsVec;
    for (int cellIdx = 0; cellIdx < gameField_.size(); cellIdx++)
    {
        if (!gameField_.at(cellIdx).isBall())
        {
            emptyCellsVec.push_back(cellIdx);
        }
    }
    return emptyCellsVec;
}

void GameField::resize(const int newCellsNb)
{
    gameField_.clear();
    gameField_.resize(newCellsNb);
}
