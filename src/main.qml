import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import com.example.diagram 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Class Diagram Viewer")

    header: ToolBar {
        Button {
            text: qsTr("Open Header File (.h)")
            onClicked: diagramManager.openFile()
        }
    }

    ScrollView {
         anchors.fill: parent
         clip: true

         Item {
             id: diagramCanvas
             width: contentWidth
             height: contentHeight

             property real contentWidth: 0
             property real contentHeight: 0

             Repeater {
                 id: connectionRepeater
                 model: diagramManager.connectionsModel
                 delegate: InheritanceLine {
                    x1: modelData.x1
                    y1: modelData.y1
                    x2: modelData.x2
                    y2: modelData.y2
                 }
             }

             Repeater {
                 id: classRepeater
                 model: diagramManager.classesModel
                 delegate: ClassBox {
                     boxX: modelData.x
                     boxY: modelData.y
                     boxWidth: modelData.width
                     boxHeight: modelData.height
                     className: modelData.name

                      Component.onCompleted: diagramCanvas.updateCanvasBounds(modelData.x + modelData.width, modelData.y + modelData.height)
                      onBoxXChanged: diagramCanvas.updateCanvasBounds(modelData.x + modelData.width, modelData.y + modelData.height)
                      onBoxYChanged: diagramCanvas.updateCanvasBounds(modelData.y + modelData.height, modelData.y + modelData.height)
                 }
             }

             function updateCanvasBounds(newX, newY) {
                 diagramCanvas.contentWidth = Math.max(diagramCanvas.contentWidth, newX + 50);
                 diagramCanvas.contentHeight = Math.max(diagramCanvas.contentHeight, newY + 50);
             }

              Connections {
                    target: diagramManager
                    function onClassesModelChanged() {
                        diagramCanvas.contentWidth = 0
                        diagramCanvas.contentHeight = 0
                    }
              }
         }
    }
}