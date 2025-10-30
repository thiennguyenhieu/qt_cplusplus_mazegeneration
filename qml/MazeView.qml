import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    property var mazeData: []
    property var startPosition: [0, 0]
    property var endPosition: [0, 0]
    property var pathSolved: []
    property var pathVisited: []

    property color backgroundColor: "#111"
    property color wallColor: "#eee"
    property color startColor: "limegreen"
    property color endColor: "red"
    property color solvedColor: "#44aaff"
    property color visitedColor: "#ffaa00aa"

    onMazeDataChanged: canvas.requestPaint()
    onStartPositionChanged: canvas.requestPaint()
    onEndPositionChanged: canvas.requestPaint()
    onPathSolvedChanged: canvas.requestPaint()
    onPathVisitedChanged: canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d")
            ctx.reset()

            ctx.fillStyle = backgroundColor
            ctx.fillRect(0, 0, width, height)

            if (root.mazeData.length === 0 || !Array.isArray(root.mazeData[0])) {
                console.log("MazeView: no data to draw")
                return
            }

            var rows = root.mazeData.length
            var cols = root.mazeData[0].length
            if (rows === 0 || cols === 0) return

            var cw = width / cols
            var ch = height / rows
            var radius = Math.min(cw, ch) / 2.5  // smaller circle radius

            // --- Draw visited path ---
            ctx.fillStyle = visitedColor
            for (var i = 0; i < root.pathVisited.length; ++i) {
                var v = root.pathVisited[i]
                if (v.length < 2) continue
                var vx = v[1] * cw + cw / 2
                var vy = v[0] * ch + ch / 2
                ctx.beginPath()
                ctx.arc(vx, vy, radius * 0.6, 0, Math.PI * 2)
                ctx.fill()
            }

            // --- Draw solved path ---
            ctx.fillStyle = solvedColor
            for (var j = 0; j < root.pathSolved.length; ++j) {
                var s = root.pathSolved[j]
                if (s.length < 2) continue
                var sx = s[1] * cw + cw / 2
                var sy = s[0] * ch + ch / 2
                ctx.beginPath()
                ctx.arc(sx, sy, radius * 0.7, 0, Math.PI * 2)
                ctx.fill()
            }

            // --- Draw start and end circles ---
            if (root.startPosition.length === 2) {
                ctx.fillStyle = startColor
                var sx = root.startPosition[1] * cw + cw / 2
                var sy = root.startPosition[0] * ch + ch / 2
                ctx.beginPath()
                ctx.arc(sx, sy, radius, 0, Math.PI * 2)
                ctx.fill()
            }

            if (root.endPosition.length === 2) {
                ctx.fillStyle = endColor
                var ex = root.endPosition[1] * cw + cw / 2
                var ey = root.endPosition[0] * ch + ch / 2
                ctx.beginPath()
                ctx.arc(ex, ey, radius, 0, Math.PI * 2)
                ctx.fill()
            }

            // --- Draw walls last (on top) ---
            ctx.strokeStyle = wallColor
            ctx.lineWidth = 1
            for (var r = 0; r < rows; ++r) {
                for (var c = 0; c < cols; ++c) {
                    var walls = root.mazeData[r][c]
                    var x = c * cw
                    var y = r * ch
                    ctx.beginPath()
                    if (walls & 1) ctx.moveTo(x, y), ctx.lineTo(x + cw, y) // North
                    if (walls & 2) ctx.moveTo(x + cw, y), ctx.lineTo(x + cw, y + ch) // East
                    if (walls & 4) ctx.moveTo(x, y + ch), ctx.lineTo(x + cw, y + ch) // South
                    if (walls & 8) ctx.moveTo(x, y), ctx.lineTo(x, y + ch) // West
                    ctx.stroke()
                }
            }
        }
    }
}
