import Qt 4.7
import com.meego 1.0
import com.meego.themebridge 1.0

ListItem {
    id: listItem
    property alias title: text.text
    property alias subtitle: subtext.text
    property alias image: delegateImage.source

    Row {
        anchors.fill: listItem.padding

        Image {
            id: delegateImage
            visible: source!=""
        }
        Column {
            anchors.verticalCenter: parent.verticalCenter

            Label {
                id: text
                text: ""
                styleObjectName: "CommonTitle"
            }

            Label {
                id: subtext
                visible: text!=""
                styleObjectName: "CommonSubTitle"
            }
        }
    }
}
