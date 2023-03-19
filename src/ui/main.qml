import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: Screen.width / 2
    height: Screen.width / 2
    visible: true
    title: qsTr("Color Lines")

    Field {
        id: gameField
        anchors.fill: parent
    }
}
