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
        spacing: 23;

        Label {
            id: title;
            anchors.top: parent.top;
            anchors.topMargin: 15;
            anchors.horizontalCenter: parent.horizontalCenter;
            text: "<b>SETTINGS</b>";
            font.pixelSize: 30;
            color: "white";
        }

        Image {
            id: separator;
            height: 6;
            anchors.left: parent.left;
            anchors.right: parent.right;
            source: "images/separator.png";
        }

        Row {
            id: sliderRow;
            spacing: 35;
            anchors.horizontalCenter: parent.horizontalCenter;

            Label {
                id: sliderLabel;
                text: "Slower";
                color: "white";
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
                color: "white";
                anchors.verticalCenter: parent.verticalCenter;
            }
        }

        Image {
            id: separator3;
            height: 6;
            anchors.left: parent.left;
            anchors.right: parent.right;
            source: "images/separator.png";
        }

        Row {
            id: buttonsRow;
            spacing: 5;
            anchors.horizontalCenter: parent.horizontalCenter;

            Button {
                id: button1;
                text: "Fade";
                checked: true;
                checkable: true;
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
