import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Item {
    id: root

    property alias newGameButtonText: newGameButton.text
    property alias scoreLabelText: scoreLabel.text
    property alias endGameText: endGameText.text

    property int scores: 0
    property bool isGameActive: true

    onScoresChanged: {
        hideAnimator.start()
    }

    Row {
        id: scoreRow

        width: parent.width
        height: 50
        anchors {
            top: parent.top
        }

        visible: isGameActive

        Label {
            id: scoreLabel
            width: contentWidth
            font.pixelSize: 30
        }

        Label {
            id: scoreValueText
            font.pixelSize: 30
            width: contentWidth

            OpacityAnimator {
                id: hideAnimator
                from: 1
                to: 0
                duration: 150
                target: scoreValueText

                onStopped: {
                    scoreValueText.text = root.scores
                    showAnimator.start()
                }
            }
            OpacityAnimator {
                id: showAnimator
                from: 0
                to: 1
                duration: 150
                target: scoreValueText
            }
        }
    }

    Button {
        id : newGameButton

        width: parent.width
        height: 50
        font.pixelSize: 30
        anchors {
            top: scoreRow.bottom
        }

        onClicked: {
            fieldModel.onNewGameClicked();
        }
    }

    Rectangle {
        id: endGameRect

        width: parent.width
        height: 50
        visible: !root.isGameActive

        anchors.centerIn: parent

        Text {
            id: endGameText
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    Component.onCompleted: {
        hideAnimator.start()
    }
}
