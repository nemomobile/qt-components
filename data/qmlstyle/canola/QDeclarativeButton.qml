import Qt 4.7
import Components 1.0

Item {
    id: button;
    width: 166;
    height: 62;

    BorderImage {
        id: releasedImage;
        anchors.fill: parent;
        visible: !sunken();
        source: "images/button_up.png";

        border.left: 17;
        border.right: 17;
        border.top: 17;
        border.bottom: 17;
    }

    BorderImage {
        id: pressedImage;
        anchors.fill: parent;
        visible: sunken();
        source: "images/button_down.png";

        border.left: 17;
        border.right: 17;
        border.top: 17;
        border.bottom: 17;
    }

    Text {
        id: label;
        anchors.fill: parent;

        horizontalAlignment: "AlignHCenter";
        verticalAlignment: "AlignVCenter";

        font.family: "Nokia Sans";
        font.pixelSize: 28;
        color: sunken() ? "#242424" : "#504F4F";

        text: component.text;
    }

    function sunken()
    {
        return model.buttonDown || model.checked;
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
    }

    Binding { target: model; property: "mousePressed"; value: mouseArea.pressed }
    Binding { target: model; property: "mouseOver"; value: mouseArea.containsMouse }
}
