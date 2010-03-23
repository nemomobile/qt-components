import Qt 4.6
import Components 1.0

Item {
    id: button;
    width: 200;
    height: 64;

    BorderImage {
        id: releasedImage;
        anchors.fill: parent;
        visible: !model.buttonDown && !model.checked;
        source: "images/button-normal.png"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: pressedImage;
        anchors.fill: parent;
        visible: model.buttonDown;
        source: "images/button-pressed.png"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: selectedImage;
        anchors.fill: parent;
        visible: !model.buttonDown && model.checked;
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

        text: component.text;
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
    }

    Binding { target: model; property: "mousePressed"; value: mouseArea.pressed; }
    Binding { target: model; property: "mouseOver"; value: mouseArea.containsMouse; }
}
