import Qt 4.7
import "UIConstants.js" as UI

ListItem {
    id: listItem
    property alias title: mainText.text
    property alias subtitle: subText.text
    property alias image: delegateImage.source

    Row {
        anchors.fill: listItem.padding
        anchors.leftMargin: 18

        Image {
            id: delegateImage
            visible: source != ""
        }
        Column {
            anchors.verticalCenter: parent.verticalCenter

            Label {
                id: mainText
                text: ""
                font.weight: Font.Bold
                font.pixelSize: 26
            }

            Label {
                id: subText
                text: ""
                font.weight: Font.Light
                font.pixelSize: 22
                color: UI.COLOR_SECONDARY_FOREGROUND

                visible: text != ""
            }
        }
    }
}
