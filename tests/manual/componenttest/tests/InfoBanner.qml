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
            spacing: platformStyle.paddingMedium

            Button {
                text: "Show"
                onClicked: { clicked.text = ""; banner.open() }

            }

            Button {
                text: "Hide"
                onClicked: banner.close();
            }

            Button {
                text: bgButton.visible ? "Hide background" : "Show background"
                onClicked: bgButton.visible = !bgButton.visible;
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

    Button {
        id: bgButton
        parent: internal.rootObject()
        visible: false
        z: 500
        text: "Background button"
    }

    QtObject {
        id: internal

        function rootObject() {
            var next = container.parent
            while (next && next.parent)
                next = next.parent
            return next
        }
    }
}
