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
        //TODO: shouldn't this be "Style.Widget" or "Style.Background"?
        styleType: Style.Label
        mode: Style.DefaultMode
    }

    ScalableImage {
        id: meegoDelegateBackground
        style: meegostyle
        imageProperty: "backgroundBaseImage"
        anchors.fill: parent
    }


    Row {
        anchors.fill: parent
        spacing: meegostyle.paddingLeft+meegostyle.paddingRight
        Image {
            id: delegateImage
            source: "images/user_picture_blank.png"
        }
        Column {
            height: parent.height
            spacing: meegostyle.paddingTop+meegostyle.paddingBottom

            Text {
                id: text
                text: "text"
                color: meegostyle.textColor
                font: meegostyle.font
            }
            Text {
                id: subtext
                text: "subtext"
                color: "gray"
                font.family: meegostyle.font.family
                font.pixelSize: meegostyle.font.pixelSize/2
            }
        }
    }
}
