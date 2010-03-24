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
            color: "black";
        }

        Image {
            id: separator;
            height: 2;
            anchors.left: parent.left;
            anchors.right: parent.right;
            source: "images/splitter.png";
        }

        Row {
            id: buttons1;
            spacing: 10;
            anchors.horizontalCenter: parent.horizontalCenter;

            Button {
                id: button11;
                text: "Effects";
            }

            Button {
                id: button12;
                text: "Effects";
            }

            Button {
                id: button13;
                text: "Effects";
            }
        }

        Image {
            id: separator2;
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
                text: "Effects";
                checked: true;
                autoExclusive: true;
            }

            Button {
                id: button2;
                text: "Effects";
                autoExclusive: true;
            }

            Button {
                id: button3;
                text: "Effects";
                autoExclusive: true;
            }
        }
    }
}