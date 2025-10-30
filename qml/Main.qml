import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 860
    height: 900
    color: "#111"
    title: qsTr("Maze Generator")

    // --- Control bar row ---
    RowLayout {
        id: controlBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        spacing: 12

        // --- Buttons on the left ---
        Button {
           text: "Generate Maze"
           font.bold: true
           implicitWidth: 140
           implicitHeight: 34
           background: Rectangle {
               color: "white"
               radius: 6
               border.color: "#aaa"
               border.width: 1
           }
           onClicked: mazeViewModel.generateMaze()
       }

        ComboBox {
            id: combo
            model: ["DFS", "BFS"]
            implicitWidth: 100
            implicitHeight: 34
            background: Rectangle {
                color: "white"
                border.color: "#aaa"
                radius: 6
            }
        }

        Button {
            text: "Solve"
            font.bold: true
            implicitWidth: 100
            implicitHeight: 34
            background: Rectangle {
                color: "white"
                radius: 6
                border.color: "#aaa"
                border.width: 1
            }
            onClicked: mazeViewModel.solveMaze(combo.currentIndex)
        }

        // --- Spacer pushes hints to the right ---
        Item { Layout.fillWidth: true }

        // --- Color hint legend (right-aligned) ---
        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 8

            RowLayout {
                spacing: 5
                Rectangle { width: 14; height: 14; radius: 7; color: "limegreen" }
                Text { text: "Start"; color: "white"; font.pixelSize: 12 }
            }
            RowLayout {
                spacing: 5
                Rectangle { width: 14; height: 14; radius: 7; color: "red" }
                Text { text: "End"; color: "white"; font.pixelSize: 12 }
            }
            RowLayout {
                spacing: 5
                Rectangle { width: 14; height: 14; radius: 7; color: "#ffaa00aa" }
                Text { text: "Visited"; color: "white"; font.pixelSize: 12 }
            }
            RowLayout {
                spacing: 5
                Rectangle { width: 14; height: 14; radius: 7; color: "#44aaff" }
                Text { text: "Solved"; color: "white"; font.pixelSize: 12 }
            }
        }
    }

    // --- Maze Area ---
    MazeView {
        id: mazeView
        anchors.top: controlBar.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: 800
        height: 800

        mazeData: mazeViewModel.mazeData
        startPosition: mazeViewModel.startPosition
        endPosition: mazeViewModel.endPosition
        pathSolved: mazeViewModel.pathSolved
        pathVisited: mazeViewModel.pathVisited
    }
}
