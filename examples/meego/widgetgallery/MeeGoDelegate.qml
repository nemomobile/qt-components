import Qt 4.7
import com.meego 1.0
import com.meego.themebridge 1.0

Item {

    property alias image: delegateImage.source
    property alias text: text.text
    property alias subtext: subtext.text

    width: parent.width

    Style {
        id: meegostyle
        // TODO: shouldn't this be a style for Widget, Background or a specific style for renderer/delegate?
        styleClass: "MLabelStyle"
    }

    ScalableImage {
        id: meegoDelegateBackground
        style: meegostyle
        imageProperty: "backgroundBaseImage"
        anchors.fill: parent
    }


    Row {
        anchors.fill: parent
        spacing: meegostyle.current.paddingLeft + meegostyle.current.paddingRight
        Image {
            id: delegateImage
            source: "images/user_picture_blank.png"
        }
        Column {
            height: parent.height
            spacing: meegostyle.current.paddingTop + meegostyle.current.paddingBottom

            Text {
                id: text
                text: "text"
                color: meegostyle.current.color
                font: meegostyle.current.font
            }
            Text {
                id: subtext
                text: "subtext"
                color: "gray"
                font.family: meegostyle.current.font.family
                font.pixelSize: meegostyle.current.font.pixelSize/2
            }
        }
    }
}
