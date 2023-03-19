import QtQuick 2.12

GridView {
    id: root

    interactive: false

    delegate: Rectangle {
        width: cellWidth
        height: cellHeight
        border.width: 1
        border.color: "black"
    }
}
