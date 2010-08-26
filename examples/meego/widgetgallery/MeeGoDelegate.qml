import Qt 4.7
import com.meego 1.0
import com.meego.themebridge 1.0

Item {
    property alias text: text.text
    property alias subtext: subtext.text
    property alias image: delegateImage.source

    width: parent.width

    Style {
        id: itemStyle
        styleClass: "MListItemStyle"
        mode: mouseArea.pressed ? "pressed" : "default"
    }

    Background {
        style: itemStyle
        anchors.fill: parent
    }

    Row {
        anchors.fill: parent
        spacing: itemStyle.current.get("paddingLeft") + itemStyle.current.get("paddingRight")

        Image {
            id: delegateImage
            source: "images/user_picture_blank.png"
        }
        Column {
            height: parent.height
            spacing: itemStyle.current.get("paddingTop") + itemStyle.current.get("paddingBottom")

            Label {
                id: text
                text: "text"
                styleObjectName: "CommonTitle"
            }

            Label {
                id: subtext
                text: "subtext"
                styleObjectName: "CommonSubTitle"
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
}
