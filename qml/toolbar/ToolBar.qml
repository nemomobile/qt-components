import Qt 4.6

Image {
    id: toolbar
    property int topMargin: 8
    width: toolButtons.width + toolButtons.x
    height: toolButtons.height
    source: "images/ToolBar_portrait.png"
    fillMode: "TileHorizontally"
    property bool enabled: true

}
