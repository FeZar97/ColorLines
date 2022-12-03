import QtQuick 2.12

GridView {
    id: root

    interactive: false

    property int clickedCellIdx: -1

    delegate: Ball {
        id: ball
        width: fieldView.cellWidth
        height: fieldView.cellHeight

        isBallVisible: display
        ballColor: decoration
        isSelected: root.currentIndex === index

        onCellClicked: {
            root.clickedCellIdx = index

            if (root.model.prevSelectedCell === -1
                    || !root.model.isAvailableMove(index)) {
                root.model.onCellClicked(index)
                return
            }

            xComponent.from = root.contentItem.children[root.model.prevSelectedCell].x
            yComponent.from = root.contentItem.children[root.model.prevSelectedCell].y

            xComponent.to = root.contentItem.children[index].x
            yComponent.to = root.contentItem.children[index].y

            ballMoveAnimation.start()
        }
    }

    SequentialAnimation {
        id : ballMoveAnimation

        ParallelAnimation {
            NumberAnimation {
                id: xComponent
                target: root.currentItem
                properties: "x"
                duration: 200
            }

            NumberAnimation {
                id: yComponent
                target: root.currentItem
                properties: "y"
                duration: 200
            }
        }

        PauseAnimation {
            duration: 20
        }

        onStopped: {
            root.model.onCellClicked(root.clickedCellIdx)
        }
    }
}
