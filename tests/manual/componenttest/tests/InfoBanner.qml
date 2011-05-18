import QtQuick 1.0
import com.nokia.symbian 1.0
import com.nokia.extras 1.0

Item {
    id: container

    Text {
        id: title
        anchors { top: parent.top; topMargin: platformStyle.paddingLarge }
        color: platformStyle.colorNormalLight
        text: "Create Custom InfoBanner"
        font.bold: true
    }

    Column {
        anchors { top: title.bottom; left: parent.left; right: parent.right; margins: platformStyle.paddingLarge }
        spacing: platformStyle.paddingMedium

        TextArea {
            id: textArea
            width: parent.width
            text: "Information to be shown to user"
        }

        Row {
            spacing: platformStyle.paddingMedium
            CheckBox {
                id: iconCheckBox
                checked: true
                text: "Enable icon"
            }
            CheckBox {
                id: interactive
                checked: false
                text: "Interactive"
            }
            Text {
                id: clicked
                color: "green"
            }
        }

        Row {
            spacing: platformStyle.paddingSmall
            Text { text: "Time to dismiss:"; color: platformStyle.colorNormalLight }
            Text { text: durationSlider.value; color: "green" }
            Text { text: "sec"; color: "green" }
            Slider { id: durationSlider; maximumValue: 9; minimumValue: 0; stepSize:1; value: 3 }
        }

        Row {
            spacing: platformStyle.paddingLarge

            Button {
                text: "Show"
                onClicked: { clicked.text = ""; banner.open() }

            }

            Button {
                text: "Hide"
                onClicked: banner.close();
            }
        }
    }

    InfoBanner {
        id: banner

        text: textArea.text
        iconSource: iconCheckBox.checked ? "qrc:info_banner_thumbnail.png" : ""
        timeout: durationSlider.value*1000
        interactive: interactive.checked
        onClicked: clicked.text = "clicked"
    }
}
