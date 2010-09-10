import Qt 4.7
import com.meego 1.0
import com.meego.themebridge 1.0

ImplicitSizeItem {
    property Component nextPage
    property alias style: itemStyle

    implicitWidth: parent.width
    implicitHeight: itemStyle.preferredHeight

    Style {
        id: itemStyle
        styleClass: "MListItemStyle"
        mode: mouseArea.pressed ? "pressed" : "default"
    }

    Background {
        style: itemStyle
        anchors.fill: parent
    }
}
