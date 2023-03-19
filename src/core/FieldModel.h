#pragma once

#include <QAbstractListModel>

#include "DbWrapper.h"
#include "GameField.h"

namespace ColorLines
{

class FieldModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    const static int cFieldSideSize{ 9 };
    const static int cCellsNb{ cFieldSideSize * cFieldSideSize };
    const static int cProducedBallsPerTurn{ 3 };
    const static int cCompleteLineLength{ 5 };
    const static int cCompleteLineAward{ 10 };
    const static int cInvalidCellIdx{ -1 };

    explicit FieldModel(QObject *parent = 0);
    ~FieldModel();

    Q_PROPERTY(int sideCellsNb READ sideCellsNb NOTIFY sideCellsNbChanged)
    Q_PROPERTY(int scores READ scores WRITE setScore NOTIFY scoresChanged)
    Q_PROPERTY(bool isGameActive READ isGameActive WRITE setGameActive NOTIFY gameActiveChanged)
    Q_PROPERTY(int prevSelectedCell READ prevSelectedCell NOTIFY prevSelectedCellChanged)

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    int sideCellsNb() const;
    int scores() const;
    bool isGameActive() const;
    int prevSelectedCell() const;

    void setScore(int newScores);
    void setGameActive(bool isGameActive);

    Q_INVOKABLE void onCellClicked(int idx);
    Q_INVOKABLE void onNewGameClicked();
    Q_INVOKABLE bool isAvailableMove(int idx) const;

signals:
    void sideCellsNbChanged(int newSideCellsNb);
    void scoresChanged(int newScores);
    void gameActiveChanged(bool isGameActive);
    void prevSelectedCellChanged(int idx);

    void moveBall(int sourceIdx, int destIdx);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    void reset(int scores = 0, const QByteArray& fieldState = {});
    void generateBalls();
    void completeLines(int initialPositionIdx);

private:
    GameField gameField_;
    int scores_{ 0 };
    int prevSelectedCellIdx_{ cInvalidCellIdx };
    bool isGameActive_{true};

    DbWrapper db_;
};

}
