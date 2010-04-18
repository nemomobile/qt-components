import Qt 4.7
import Mx 1.0

Item {
    Row {
        id: row
        x: 20; y: 50
        spacing: 4

        Button {
            text: "Add crumb"
            onClicked: { bar.push("Crumb" + (bar.level + 1)); }
        }

        Button {
            text: "Remove crumb"
            onClicked: { bar.pop(); }
        }

        Button {
            text: "Toggle editable"
            onClicked: { bar.editable = !bar.editable; }
        }

        Button {
            text: "Re-label 1st button"
            onClicked: { bar.setLabel(1, bar.entry.text); }
        }
    }

    Rectangle {
        color: "gainsboro"
        anchors.fill: bar
    }

    PathBar {
        id: bar

        anchors.top: row.bottom
        anchors.topMargin: 20
        anchors.left: row.left

        width: parent.width - 40
        clearOnChange: true
    }
}
