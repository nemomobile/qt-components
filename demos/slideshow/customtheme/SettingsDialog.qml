import Qt 4.6

Rectangle {
    id: settingsDialog;
    color: "black";
    opacity: 0.7;

    Label {
        id: title;
        anchors.top: parent.top;
        anchors.topMargin: 15;
        anchors.horizontalCenter: parent.horizontalCenter;
        text: "<b>SETTINGS</b>";
        color: "white";
    }

    Rectangle {
        id: separator;
        color: "red";
        height: 6;
        anchors.top: title.bottom;
        anchors.left: parent.left;
        anchors.right: parent.right;
    }

    Row {
        id: row;
        spacing: 30;

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

        Rectangle {
            id: separator2;
            color: "red";
            width: 6;
            anchors.top: parent.top;
            anchors.bottom: parent.bottom;
        }

        Column {
            id: sliderColumn;
            anchors.verticalCenter: row.verticalCenter;

            Slider {
                id: slider;
                width: 200;
                height: 200;
            }

            Label {
                id: value;
                color: "white";
                text: slider.value;
                anchors.horizontalCenter: slider.horizontalCenter;
            }
        }
    }
}
