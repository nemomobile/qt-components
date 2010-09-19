import Qt 4.7
import com.meego.themebridge 1.0

ListItem {
    id: listItem
    property alias title: mainText.text
    property alias subtitle: subText.text
    property alias image: delegateImage.source

    Row {
        anchors.fill: listItem.padding

        Image {
            id: delegateImage
            visible: source != ""
        }
        Column {
            anchors.verticalCenter: parent.verticalCenter

            Label {
                id: mainText
                text: ""
                styleObjectName: "CommonTitle"
            }

            Label {
                id: subText
                text: ""
                visible: text != ""
                styleObjectName: "CommonSubTitle"
            }
        }
    }
}
