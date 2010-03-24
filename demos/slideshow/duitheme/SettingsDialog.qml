import Qt 4.6

BorderImage {
    id: settingsDialog;
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
            source: "images/separator.svg";
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
            }

            Label {
                id: sliderValue;
                text: "Faster";
                anchors.verticalCenter: parent.verticalCenter;
            }
        }

        Image {
            id: separator3;
            height: 6;
            anchors.left: parent.left;
            anchors.right: parent.right;
            source: "images/separator.svg";
        }

        Row {
            id: buttonsRow;
            spacing: 5;
            anchors.horizontalCenter: parent.horizontalCenter;

            Button {
                id: button1;
                text: "Fade";
                checked: true;
                autoExclusive: true;
                onClicked: viewer.state = "fade";
            }

            Button {
                id: button2;
                text: "Sliding";
                checkable: true;
                autoExclusive: true;
                onClicked: viewer.state = "sliding";
            }

            Button {
                id: button3;
                text: "Transform";
                checkable: true;
                autoExclusive: true;
                onClicked: viewer.state = "transform";
            }
        }

    }
}
