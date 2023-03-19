#include "Cell.h"

#include <QRandomGenerator>

using namespace ColorLines;

Cell::Cell(bool isBall, quint32 rgbColor)
{
    data_ = (isBall ? 0xFF000000 : 0) | rgbColor;
}

Cell::Cell(const quint32 data)
    : data_{data}
{
}

Cell::Cell(const Cell &rhs)
    : data_{rhs.data_}
{
}

bool Cell::operator==(const Cell &rhs) const
{
    return data_ == rhs.data_;
}

bool Cell::isBall() const
{
    return data_ & 0xFF000000;
}

quint32 Cell::getColor() const
{
    return data_ & 0x00FFFFFF;
}

QByteArray Cell::getBytes() const
{
    QByteArray byteArray(4, 0);
    memcpy_s(byteArray.data(), byteArray.size(), &data_, sizeof(data_));
    return byteArray;
}

void ColorLines::Cell::setState(const QByteArray& byteArray)
{
    Q_ASSERT(byteArray.size() == sizeof(Cell));
    memcpy_s(&data_, sizeof(data_), byteArray.data(), byteArray.size());
}

Cell Cell::generateBallWithRandomColor()
{
    quint32 randomColor;
    switch (QRandomGenerator::global()->bounded(0, cBallColorsNb + 1))
    {
    case 0:
        randomColor = Red;
        break;
    case 1:
        randomColor = Green;
        break;
    case 2:
        randomColor = Blue;
        break;
    case 3:
    default:
        randomColor = Yellow;
        break;
    }

    return Cell(true, randomColor);
}
