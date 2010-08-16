import Qt 4.7
import com.meego 1.0
import com.meego.themebridge 1.0

Rectangle {

    property alias image: delegateImage.source
    property alias text: text.text
    property alias subtext: subtext.text

    border.color: "gray"
    border.width: 1

    Style {
        id: meegostyle
        styleType:  Style.Button
    }

    Background {
        id: meegoDelegateBackground
        style: meegostyle
        anchors.fill: parent
    }

    Row {
        height: parent.height
        width: parent.width
        Image {
            id: delegateImage
        }
        Column {
            height: parent.height

            Text {
                id: text
                text: "text"
                color: meegostyle.textColor
                font: meegostyle.font
            }
            Text {
                id: subtext
                text: "subtext"
                color: meegostyle.textColor
                font: meegostyle.font
            }
        }
    }
}
