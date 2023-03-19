#include "FieldModel.h"

#include <QColor>
#include <QRandomGenerator>
#include <QSet>

#include <QDebug>

ColorLines::FieldModel::FieldModel(QObject *parent)
    : QAbstractItemModel(parent), gameField_(cCellsNb)
{
    int restoredScores = db_.getPrevScores();
    QByteArray restoredFieldState = db_.getPrevFieldState();

    reset(restoredScores, restoredFieldState);
}

ColorLines::FieldModel::~FieldModel()
{
    db_.saveState(scores_, gameField_.getState());
}

int ColorLines::FieldModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return gameField_.getCellsNb();
}

int ColorLines::FieldModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return 1;
}

QModelIndex ColorLines::FieldModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (row >=0 && row < gameField_.getCellsNb())
    {
        return createIndex(row, column);
    }

    return {};
}

QVariant ColorLines::FieldModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }
    const Cell& cell = gameField_.getCell(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return { cell.isBall() };
    case Qt::DecorationRole:
        return { QColor(cell.getColor()) };
    }

    return {};
}

QModelIndex ColorLines::FieldModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return {};
}

int ColorLines::FieldModel::sideCellsNb() const
{
    return static_cast<int>(sqrt(gameField_.getCellsNb()));
}

int ColorLines::FieldModel::scores() const
{
    return scores_;
}

bool ColorLines::FieldModel::isGameActive() const
{
    return isGameActive_;
}

int ColorLines::FieldModel::prevSelectedCell() const
{
    return prevSelectedCellIdx_;
}

void ColorLines::FieldModel::setScore(int newScores)
{
    if (scores_ == newScores)
    {
        return;
    }

    scores_ = newScores;
    emit scoresChanged(scores_);
}

void ColorLines::FieldModel::setGameActive(bool isGameActive)
{
    if (isGameActive_ == isGameActive)
    {
        return;
    }

    isGameActive_ = isGameActive;
    emit gameActiveChanged(isGameActive_);
}

void ColorLines::FieldModel::onCellClicked(int idx)
{
    QModelIndex cellIndex = index(idx, 1);
    if (!cellIndex.isValid())
    {
        return;
    }

    bool isBall = gameField_.getCell(idx).isBall();

    // selected cell contains ball
    if (isBall)
    {
        // forget last selected cell
        if (idx == prevSelectedCellIdx_)
        {
            prevSelectedCellIdx_ = cInvalidCellIdx;
        }
        // save last selected cell with ball
        else
        {
            prevSelectedCellIdx_ = idx;
        }
    }
    // selected cell didn't contain ball
    else
    {
        // exist previously selected cell with ball
        if (prevSelectedCellIdx_ != cInvalidCellIdx)
        {
            gameField_.setCellState(idx, gameField_.getCell(prevSelectedCellIdx_).getBytes());
            gameField_.setCellState(prevSelectedCellIdx_, {sizeof(Cell), 0});

            QModelIndex changedModelIndex = index(idx, 0);
            emit dataChanged(changedModelIndex, changedModelIndex);

            completeLines(idx);

            generateBalls();

            setGameActive(!gameField_.getEmptyCells().empty());
        }
        prevSelectedCellIdx_ = cInvalidCellIdx;
    }
    emit prevSelectedCellChanged(prevSelectedCellIdx_);
}

void ColorLines::FieldModel::onNewGameClicked()
{
    reset();
}

bool ColorLines::FieldModel::isAvailableMove(int idx) const
{
    return !gameField_.getCell(idx).isBall();
}

QHash<int, QByteArray> ColorLines::FieldModel::roleNames() const
{
    return QAbstractItemModel::roleNames();
}

void ColorLines::FieldModel::reset(int scores, const QByteArray &fieldState)
{
    setScore(scores);
    prevSelectedCellIdx_ = cInvalidCellIdx;

    beginResetModel();
    if (fieldState.isEmpty())
    {
        gameField_.reset();
    }
    else
    {
        gameField_.setState(cCellsNb, fieldState);
    }
    endResetModel();

    setGameActive(!gameField_.getEmptyCells().empty());

    if (isGameActive() && fieldState.isEmpty())
    {
        generateBalls();
    }
}

void ColorLines::FieldModel::generateBalls()
{
    QVector<int> emptyCells = gameField_.getEmptyCells();
    if (emptyCells.size() <= cProducedBallsPerTurn)
    {
        for (const int newBallIdx: emptyCells)
        {
            beginResetModel();
            gameField_.setCellState(newBallIdx, Cell::generateBallWithRandomColor().getBytes());
            endResetModel();
            // FIXME
            // emit dataChanged(index(newBallIdx, 0), index(newBallIdx, 0));

            completeLines(newBallIdx);
        }
    }
    else
    {
        QSet<int> emptyCellIdxs;
        while (emptyCellIdxs.size() != cProducedBallsPerTurn)
        {
            emptyCellIdxs.insert(QRandomGenerator::global()->bounded(0, emptyCells.size()));
        }

        for (const int emptyCellIdx: emptyCellIdxs)
        {
            int newBallIdx = emptyCells.at(emptyCellIdx);
            beginResetModel();
            gameField_.setCellState(newBallIdx, Cell::generateBallWithRandomColor().getBytes());
            endResetModel();
            // FIXME
            // emit dataChanged(index(newBallIdx, 0), index(newBallIdx, 0));

            completeLines(newBallIdx);
        }
    }
}

// lines of 5 cells are checked in order, in which the current cell moves from the beginning to the end
void ColorLines::FieldModel::completeLines(int initialPositionIdx)
{
    quint32 sourceColor = gameField_.getCell(initialPositionIdx).getColor();
    if (!sourceColor)
    {
        return;
    }

    bool isLineExist = false;
    QVector<int> completeLineCoords(cCompleteLineLength);

    for (int verticalLineIdx = 0; verticalLineIdx < cCompleteLineLength; verticalLineIdx++)
    {
        int curLineStartIdx = initialPositionIdx - (cCompleteLineLength - 1 - verticalLineIdx) * cFieldSideSize;
        if (curLineStartIdx < 0)
        {
            continue;
        }

        int curLineEndIdx = initialPositionIdx + verticalLineIdx * cFieldSideSize;
        if (curLineEndIdx >= gameField_.getCellsNb())
        {
            continue;
        }

        // check colors
        bool isLineComplete = true;
        for (int cellIdx = 0; cellIdx < cCompleteLineLength; cellIdx++)
        {
            completeLineCoords[cellIdx] = curLineStartIdx + cellIdx * cFieldSideSize;
            if (gameField_.getCell(completeLineCoords[cellIdx]).getColor() != sourceColor)
            {
                isLineComplete = false;
                break;
            }
        }

        if (!isLineComplete)
        {
            continue;
        }

        isLineExist = true;
        break;
    }

    if (!isLineExist)
    {
        int sourceRow = initialPositionIdx / cFieldSideSize;
        for (int horizontalLineIdx = 0; horizontalLineIdx < cCompleteLineLength; horizontalLineIdx++)
        {
            int curLineStartIdx = initialPositionIdx - (cCompleteLineLength - 1 - horizontalLineIdx);
            int curLineStartRow = curLineStartIdx / cFieldSideSize;
            if (curLineStartIdx < 0 || curLineStartRow != sourceRow)
            {
                continue;
            }

            int curLineEndIdx = initialPositionIdx + horizontalLineIdx;
            int curLineEndRow = curLineEndIdx / cFieldSideSize;
            if (curLineEndIdx >= gameField_.getCellsNb() || curLineEndRow != sourceRow)
            {
                continue;
            }

            // check colors
            bool isLineComplete = true;
            for (int cellIdx = 0; cellIdx < cCompleteLineLength; cellIdx++)
            {
                completeLineCoords[cellIdx] = curLineStartIdx + cellIdx;
                if (gameField_.getCell(completeLineCoords[cellIdx]).getColor() != sourceColor)
                {
                    isLineComplete = false;
                    break;
                }
            }

            if (!isLineComplete)
            {
                continue;
            }

            isLineExist = true;
            break;
        }
    }

    if (!isLineExist)
    {
        return;
    }

    // line [curLineStartIdx, curLineEndIdx] is complete
    setScore(scores_ + cCompleteLineAward);

    for (int coords: completeLineCoords)
    {
        gameField_.setCellState(coords, {sizeof(Cell), 0});
        emit dataChanged(index(coords, 0), index(coords, 0));
    }
}
