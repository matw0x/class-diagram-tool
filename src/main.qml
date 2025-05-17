import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import com.example.diagram 1.0
import QtQuick.Layouts 1.15

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
         id: diagramScrollView
         anchors.fill: parent
         clip: true
         ScrollBar.horizontal.policy: ScrollBar.AsNeeded
         ScrollBar.vertical.policy: ScrollBar.AsNeeded

         onContentWidthChanged: console.log("ScrollView contentWidth:", contentWidth)
         onContentHeightChanged: console.log("ScrollView contentHeight:", contentHeight)


         Item {
             id: diagramCanvas
             width: currentCalculatedWidth
             height: currentCalculatedHeight

             property real currentCalculatedWidth: diagramScrollView.width
             property real currentCalculatedHeight: diagramScrollView.height
             property int loadedItems: 0
             property int expectedItems: 0

             function resetLoadState() {
                 loadedItems = 0
                 expectedItems = diagramManager ? diagramManager.classesModel.length : 0
                 console.log("resetLoadState: expectedItems =", expectedItems)
                 if (expectedItems > 0) {
                     delayedUpdateTimer.restart()
                 } else {
                     diagramCanvas.currentCalculatedWidth = diagramScrollView.width
                     diagramCanvas.currentCalculatedHeight = diagramScrollView.height
                     updateDiagramAreaSize()
                 }
             }

             Timer {
                 id: delayedUpdateTimer
                 interval: 100
                 onTriggered: {
                     console.log("Delayed update triggered")
                     diagramCanvas.updateDiagramAreaSize()
                 }
             }

             Repeater {
                 id: connectionRepeater
                 model: diagramManager ? diagramManager.connectionsModel : []
                 delegate: InheritanceLine {
                    x1: modelData.x1
                    y1: modelData.y1
                    x2: modelData.x2
                    y2: modelData.y2
                 }
             }

             Repeater {
                 id: classRepeater
                 model: diagramManager ? diagramManager.classesModel : []
                 delegate: ClassBox {
                     boxX: modelData.x
                     boxY: modelData.y
                     boxWidth: modelData.width
                     boxHeight: modelData.height
                     className: modelData.name

                      Component.onCompleted: {
                          console.log("ClassBox loaded for index:", model.index, "x,y,w,h:", boxX, boxY, boxWidth, boxHeight)
                          diagramCanvas.loadedItems++
                          console.log("Loaded items:", diagramCanvas.loadedItems, "of", diagramCanvas.expectedItems)
                          if (diagramCanvas.loadedItems === diagramCanvas.expectedItems && diagramCanvas.expectedItems > 0) {
                              console.log("All classes loaded, updating diagramCanvas size")
                              diagramCanvas.updateDiagramAreaSize()
                          }
                      }
                      onBoxXChanged: {
                         if (diagramCanvas.expectedItems > 0) {
                             delayedUpdateTimer.restart()
                         }
                      }
                      onBoxYChanged: {
                          if (diagramCanvas.expectedItems > 0) {
                              delayedUpdateTimer.restart()
                          }
                      }
                 }
             }

             function getRequiredContentWidth() {
                 var maxX = 0
                 var children = diagramCanvas.children
                 for (var i = 0; i < children.length; ++i) {
                     var item = children[i];
                     if (item.boxX !== undefined && item.boxWidth !== undefined) {
                          maxX = Math.max(maxX, item.boxX + item.boxWidth)
                     }
                 }
                 var calculatedWidth = maxX + 50;
                 return Math.max(calculatedWidth, diagramScrollView.width);
             }

             function getRequiredContentHeight() {
                 var maxY = 0
                 var children = diagramCanvas.children
                 for (var i = 0; i < children.length; ++i) {
                     var item = children[i];
                     if (item.boxY !== undefined && item.boxHeight !== undefined) {
                         maxY = Math.max(maxY, item.boxY + item.boxHeight)
                     }
                 }
                 var calculatedHeight = maxY + 50;
                 return Math.max(calculatedHeight, diagramScrollView.height);
             }

             function updateDiagramAreaSize() {
                 var newRequiredWidth = getRequiredContentWidth()
                 var newRequiredHeight = getRequiredContentHeight()

                 console.log("Updating diagram area size. Required:", newRequiredWidth, newRequiredHeight)

                 diagramCanvas.currentCalculatedWidth = newRequiredWidth;
                 diagramCanvas.currentCalculatedHeight = newRequiredHeight;

                 diagramScrollView.contentWidth = diagramCanvas.currentCalculatedWidth
                 diagramScrollView.contentHeight = diagramCanvas.currentCalculatedHeight

                 console.log("Diagram area size updated. Current:", diagramCanvas.width, diagramCanvas.height, "ScrollView content:", diagramScrollView.contentWidth, diagramScrollView.contentHeight)
             }


              Connections {
                    target: diagramManager
                    function onClassesModelChanged() {
                        console.log("diagramManager.onClassesModelChanged triggered")
                        diagramCanvas.resetLoadState()
                    }
                    function onConnectionsModelChanged() {
                        console.log("diagramManager.onConnectionsModelChanged triggered")
                    }
              }
         }
    }
}