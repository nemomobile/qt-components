import Qt 4.7
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: listItem

    property alias tilePosition: background.tilePosition
    property alias styleObjectName: itemStyle.styleObjectName

    property alias padding: paddingRectangle

    signal clicked

    implicitWidth: parent.width
    implicitHeight: itemStyle.preferredHeight

    Style {
        id: itemStyle
        styleClass: "MListItemStyle"
        // XXX: avoid get in pressed mode if we are flicking
        mode: mouseArea.pressed ? "pressed" : "default"
    }

    Background {
        id: background
        style: itemStyle
        anchors.fill: parent
        tilePosition: Background.VerticalCenterPosition
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            listItem.clicked();
        }
    }

    Item {
        // non-visible item to create a padding boundary that
        // content items can bind to
        id: paddingRectangle
        anchors.leftMargin: itemStyle.current.get("paddingLeft")
        anchors.rightMargin: itemStyle.current.get("paddingRight")
        anchors.topMargin: itemStyle.current.get("paddingTop")
        anchors.bottomMargin: itemStyle.current.get("paddingBottom")
        anchors.fill: parent
    }
}
