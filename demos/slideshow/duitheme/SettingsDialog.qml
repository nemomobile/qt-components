import Qt 4.6

BorderImage {
    id: settingsDialog;
    source: "images/settings_background.png";

    border.left: 15;
    border.right: 15;
    border.top: 15;

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
        anchors.top: title.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
        source: "images/separator.svg";
    }

    Row {
        id: row;
        spacing: 20;

        anchors.top: separator.bottom
        anchors.bottom: settingsDialog.bottom;
        anchors.left: parent.left;
        anchors.leftMargin: 10;
        anchors.right: parent.right;
        anchors.rightMargin: 10;

        Column {
            id: buttonsColumn;
            anchors.verticalCenter: row.verticalCenter;
            spacing: 5;

            Button {
                id: button1;
                text: "Fade";
                checked: true;
                checkable: true;
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

        Column {
            id: sliderColumn;
            anchors.verticalCenter: row.verticalCenter;

            Row {
                spacing: 25;

                Label {
                    id: sliderLabel;
                    text: "Slower";
                    anchors.verticalCenter: parent.verticalCenter;
                }

                Slider {
                    id: slider;
                    width: 350;
                    height: 50;
                    minimum: 0;
                    maximum: 100;
                    value: 42;

                    // ###
                    onValueChanged: { viewer.interval = 11000 - 100*value; }
                }

                Label {
                    id: sliderValue;
                    text: "Faster";
                    anchors.verticalCenter: parent.verticalCenter;
                }
            }
        }
    }
}
