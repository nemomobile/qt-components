import Qt 4.7
import com.nokia.symbian 1.0
import com.nokia.extras 1.0

Page {
    id: container

    Rectangle {
        anchors.fill: parent
        color: "pink"
    }

    Text {
        id: title
        color: platformStyle.colorNormalLight
        text: "Create Custom InfoBanner"
        font.bold: true
    }

    Column {
        anchors { top: title.bottom; topMargin: platformStyle.paddingLarge; left: parent.left; leftMargin: platformStyle.paddingLarge }
        spacing: platformStyle.paddingMedium

        TextField {
            id: textField
            width: parent.width
            text: "Information to be shown to user"
        }

        CheckBox {
            id: iconCheckBox
            checked: true
            text: "Enable icon"
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
                onClicked: banner.open();
            }

            Button {
                text: "Hide"
                onClicked: banner.close();
            }

            Button {
                text: "More Samples"
                onClicked: {
                    var component = Qt.createComponent("InfoBannerSamples.qml")
                    if (component.status == Component.Ready)
                        root.pageStack.push(component);
                }
            }
        }
    }

    InfoBanner {
        id: banner

        text: textField.text
        iconSource: iconCheckBox.checked ? "assets/info_banner_thumbnail.png" : ""
        timeout: durationSlider.value*1000
    }
}
