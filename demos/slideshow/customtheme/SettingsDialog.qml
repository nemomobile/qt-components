import Qt 4.6

Rectangle {
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
        anchors.bottom: parent.bottom;
        anchors.left: parent.left;
        anchors.leftMargin: 10;
        anchors.right: parent.right;
        anchors.rightMargin: 10;

        Column {
            anchors.verticalCenter: row.verticalCenter;
            spacing: 5;

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
        }
    }
}
