import Qt 4.6
import Components 1.0

Item {
    id: button;

    BorderImage {
        id: releasedImage;
        anchors.fill: parent;
        visible: !model.buttonDown && !model.checked;
        source: "images/button.svg"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: pressedImage;
        anchors.fill: parent;
        visible: model.buttonDown && !model.checked;
        source: "images/button-pressed.svg"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: releasedSelectedImage;
        anchors.fill: parent;
        visible: !model.buttonDown && model.checked;
        source: "images/button-selected.svg"

        border.left: 10;
        border.right: 10;
        border.top: 10;
        border.bottom: 10;
    }

    BorderImage {
        id: pressedSelectedImage;
        anchors.fill: parent;
        visible: model.buttonDown && model.checked;
        source: "images/button-selected-pressed.svg"

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
        color: releasedImage.visible ? "#000000" : "#FFFFFF";

        text: component.text;
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
    }

    Binding { target: model; property: "mousePressed"; value: mouseArea.pressed }
    Binding { target: model; property: "mouseOver"; value: mouseArea.containsMouse }
}
