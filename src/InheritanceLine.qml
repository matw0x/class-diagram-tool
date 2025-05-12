import QtQuick 2.15

Canvas {
    id: canvas
    property real x1: 0
    property real y1: 0
    property real x2: 0
    property real y2: 0

    x: Math.min(x1, x2) - arrowSize
    y: Math.min(y1, y2) - arrowSize
    width: Math.abs(x1 - x2) + 2 * arrowSize
    height: Math.abs(y1 - y2) + 2 * arrowSize
    property real arrowSize: 8

    onPaint: {
        var ctx = getContext("2d");
        ctx.reset();

        var localX1 = x1 - x;
        var localY1 = y1 - y;
        var localX2 = x2 - x;
        var localY2 = y2 - y;

        ctx.lineWidth = 1;
        ctx.strokeStyle = "gray";
        ctx.beginPath();
        ctx.moveTo(localX1, localY1);
        ctx.lineTo(localX2, localY2);
        ctx.stroke();

        var angle = Math.atan2(localY2 - localY1, localX2 - localX1);
        var angleOffset = Math.PI / 6;

        ctx.fillStyle = "white";
        ctx.strokeStyle = "gray";
        ctx.lineWidth = 1;

        ctx.beginPath();
         ctx.moveTo(localX2, localY2);
        ctx.lineTo(localX2 - arrowSize * Math.cos(angle - angleOffset),
                   localY2 - arrowSize * Math.sin(angle - angleOffset));
         ctx.lineTo(localX2 - arrowSize * Math.cos(angle + angleOffset),
                   localY2 - arrowSize * Math.sin(angle + angleOffset));
        ctx.closePath();
        ctx.fill();
        ctx.stroke();

    }

     onX1Changed: requestPaint()
     onY1Changed: requestPaint()
     onX2Changed: requestPaint()
     onY2Changed: requestPaint()
}