import QtQuick 2.15

Rectangle {
    id: classRect
    property real boxX: 0
    property real boxY: 0
    property real boxWidth: 150
    property real boxHeight: 50
    property string className: "ClassName"

    x: boxX
    y: boxY
    width: boxWidth
    height: boxHeight
    color: "lightblue"
    border.color: "steelblue"
    border.width: 1
    radius: 3

    Text {
        anchors.centerIn: parent
        text: className
        font.pixelSize: 12
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width - 10
    }
}