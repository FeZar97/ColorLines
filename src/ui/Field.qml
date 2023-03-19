import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import FieldModel 1.0

Item {
    id: root
    anchors.fill: parent

    property real menuWidthCoef: 0.2
    property real fieldWidthCoef: 0.6
    property real defaultCellSize: 80
    property bool isGameActive: fieldModel.isGameActive

    FieldModel {
        id: fieldModel
    }

    FieldMenu {
        id: menu

        width: parent.width * menuWidthCoef
        height: parent.height
        anchors {
            top: parent.top
            left: parent.left
            margins: 20
        }

        newGameButtonText: qsTr("Новая игра")
        scoreLabelText: qsTr("Счет: ")
        endGameText: qsTr("Игра завершена\nНабранные очки: " + scores)

        scores: fieldModel.scores
        isGameActive: root.isGameActive
    }

    Rectangle {
        id: viewRect

        anchors {
            top: parent.top
            left: menu.right
            right: parent.right
            bottom: parent.bottom
            margins: 20
        }

        FieldBgView {
            id: fieldBgView

            width: model.sideCellsNb * defaultCellSize
            height: model.sideCellsNb * defaultCellSize
            anchors.centerIn: parent

            cellHeight: defaultCellSize
            cellWidth: defaultCellSize

            model: fieldModel
        }

        FieldView {
            id: fieldView

            width: model.sideCellsNb * defaultCellSize
            height: model.sideCellsNb * defaultCellSize
            anchors.centerIn: parent

            enabled: root.isGameActive

            cellWidth: defaultCellSize
            cellHeight: defaultCellSize

            currentIndex: model.prevSelectedCell

            model: fieldModel
        }
    }
}
