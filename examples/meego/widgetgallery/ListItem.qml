import Qt 4.7
import com.meego 1.0
import com.meego.themebridge 1.0

ImplicitSizeItem {
    property Component nextPage
    property alias style: itemStyle
    property alias padding: paddingRectangle

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
        // ensure that only first and last item get rounded corners
        // TODO: ensure sections also round first and last
        tilePosition: (index==ListView.view.count-1 ? Background.VerticalBottomPosition : (index == 0 ? Background.VerticalTopPosition : Background.VerticalCenterPosition))
    }

    Item {
        // non-visible item to create a padding boundary that
        // content items can bind to
        id: paddingRectangle
        anchors.leftMargin: style.current.get("paddingLeft")
        anchors.rightMargin: style.current.get("paddingRight")
        anchors.topMargin: style.current.get("paddingTop")
        anchors.bottomMargin:  style.current.get("paddingBottom")
        anchors.fill: parent
    }

}
