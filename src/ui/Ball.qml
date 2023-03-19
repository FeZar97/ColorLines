import QtQuick 2.12
import QtGraphicalEffects 1.0

Item {
    id: root

    width: parent.width
    height: parent.height
    visible: true
    opacity: ball.opacity

    enabled: parent.enabled

    property real ballSizeCoef: 0.9
    property alias ballColor: ball.color
    property int animationDuration: 300
    property bool isBallVisible: false
    property bool isSelected: false

    signal cellClicked(var mouse)

    Rectangle {
        id: ball
        width: root.width * ballSizeCoef
        height: width
        anchors.centerIn: parent
        radius: width / 2
        opacity: 0
        visible: true

        border.width: (root.isSelected && isBallVisible) ? 5 : 0
        border.color: "black"
    }

    MouseArea {
        anchors.fill: parent
        enabled: root.enabled
        onClicked: {
            root.cellClicked(mouse);
        }
    }

    onIsBallVisibleChanged: {
        if (isBallVisible) {
            showAnimation.stop();
            showAnimation.start();
        }
        else {
            showAnimation.stop();
            hideAnimation.start();
        }
    }

    PropertyAnimation{
        id: showAnimation
        target: ball
        properties: "opacity"
        duration: root.animationDuration
        from: ball.opacity
        to: 1
        onStopped: {
            ball.opacity = 1;
        }
    }

    PropertyAnimation{
        id: hideAnimation
        target: ball
        properties: "opacity"
        duration: root.animationDuration
        from: ball.opacity
        to: 0
        onStopped: {
            ball.opacity = 1;
        }
    }
}
