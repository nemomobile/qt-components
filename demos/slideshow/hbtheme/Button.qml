import Qt 4.6
import Components 1.0

Item {
    id: button;
    width: 200;
    height: 64;

    property alias checked: model.checked;
    property alias checkable: model.checkable;
    property alias autoExclusive: model.autoExclusive;
    property alias text: label.text;
    signal clicked;

    BorderImage {
        id: releasedImage;
        anchors.fill: parent;
        visible: !model.down && !model.checked;
        source: "images/button-normal.png"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: pressedImage;
        anchors.fill: parent;
        visible: model.down;
        source: "images/button-pressed.png"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: selectedImage;
        anchors.fill: parent;
        visible: !model.down && model.checked;
        source: "images/button-latched.png"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    Text {
        id: label;
        anchors.fill: parent;

        horizontalAlignment: "AlignHCenter";
        verticalAlignment: "AlignVCenter";

        font.family: "Nokia Sans";
        font.pixelSize: 24;
        color: pressedImage.visible ? "#125987" : "#FFFFFF";

        text: "Effect";
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
    }

    ButtonModel {
        id: model;

        pressed: mouseArea.pressed;
        highlighted: mouseArea.containsMouse;
        onClicked: button.clicked();
    }
}
