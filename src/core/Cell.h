#pragma once

#include <QtGlobal>
#include <QByteArray>

namespace ColorLines
{

class Cell
{

public:
    enum AvailableBallColors {
        Red = 0x00FF0000,
        Green = 0x0000FF00,
        Blue = 0x000000FF,
        Yellow = 0x00FFFF00,
    };
    const static int cBallColorsNb{ 4 };

    Cell(bool isBall = false, quint32 rgbColor = Yellow);
    Cell(const quint32 data);
    Cell(const Cell& rhs);
    bool operator==(const Cell& rhs) const;

    bool isBall() const;
    quint32 getColor() const;

    QByteArray getBytes() const;
    void setState(const QByteArray& byteArray);

    static Cell generateBallWithRandomColor();

private:
    quint32 data_;
};

}
