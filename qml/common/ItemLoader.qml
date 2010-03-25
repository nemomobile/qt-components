import Qt 4.6


Loader {
    id: itemLoader
    property alias label: label.text
    Text { id:label ;
        anchors.leftMargin:10
        color: "#888888"
        text: "LabelText";
        font.bold:true
        font.pointSize:10
        anchors.bottom:parent.top;
        anchors.left:parent.left
    }
    onStatusChanged: if (status == Loader.Ready) { stateColumn.parent = item }
    Column {
        id: stateColumn
        anchors.left: parent.right
        anchors.top: label.bottom
        anchors.leftMargin:10
        anchors.topMargin:10
        Text {
            text: "HighLighted"; font.pointSize: 9; color: highlightRegion.pressed || itemLoader.item.state=="Highlighted"?"red":"black"
            MouseArea { id: highlightRegion; anchors.fill: parent; onClicked: { itemLoader.item.state="Highlighted" } }
        }
        Text {
            text: "Disabled"; font.pointSize: 9; color: disabledRegion.pressed || itemLoader.item.state=="Disabled" ?"red":"black"
            MouseArea { id: disabledRegion; anchors.fill: parent; onClicked: { itemLoader.item.state="Disabled" } }
        }
        Text {
            text: "Normal"; font.pointSize: 9; color: normalRegion.pressed || itemLoader.item.state=="" ?"red":"black"
            MouseArea { id:normalRegion; anchors.fill: parent; onClicked: { itemLoader.item.state="" } }
        }
    }
}
