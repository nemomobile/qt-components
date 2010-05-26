import Qt 4.7

BorderImage {
    property int interval: 11000 - 100 * slider.value;
    property string animatorName;

    animatorName: {
        // ### Button group could help.
        if (button1.checked)
            "fade";
        else if (button2.checked)
            "sliding";
        else if (button3.checked)
            "transform";
    }

    source: "images/settings_background.png";
    border.left: 15;
    border.right: 15;
    border.top: 15;

    Column {
        id: settings;
        anchors.fill: parent;
        spacing: 20;

        Row {
            id: labelRow;
            height: 70;
            anchors.horizontalCenter: parent.horizontalCenter;
            Label {
                id: title;
                anchors.bottom: parent.bottom;
                text: "<b>SETTINGS</b>";
                font.pixelSize: 30;
                color: "black";
            }
        }

        Image {
            id: separator;
            height: 2;
            anchors.left: parent.left;
            anchors.right: parent.right;
            source: "images/splitter.png";
        }

        Row {
            id: sliderRow;
            spacing: 35;
            anchors.horizontalCenter: parent.horizontalCenter;

            Label {
                id: sliderLabel;
                text: "Slower";
                anchors.verticalCenter: parent.verticalCenter;
            }

            Slider {
                id: slider;
                width: 400;
                height: 50;
                value: 42;
            }

            Label {
                id: sliderValue;
                text: "Faster";
                anchors.verticalCenter: parent.verticalCenter;
            }
        }

        Image {
            id: separator3;
            height: 2;
            anchors.left: parent.left;
            anchors.right: parent.right;
            source: "images/splitter.png";
        }

        Row {
            id: buttonsRow;
            spacing: 5;
            anchors.horizontalCenter: parent.horizontalCenter;

            Button {
                id: button1;
                text: "Fade";
                checkable: true;
                checked: true;
                autoExclusive: true;
            }

            Button {
                id: button2;
                text: "Sliding";
                checkable: true;
                autoExclusive: true;
            }

            Button {
                id: button3;
                text: "Transform";
                checkable: true;
                autoExclusive: true;
            }
        }
    }
}
