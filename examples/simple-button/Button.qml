import Qt 4.7
import Components 1.0

Item {
    id: button;

    width: 80;
    height: 80;
    property alias pressedSource: pressedImage.source;
    property alias releasedSource: releasedImage.source;

    Image {
        id: releasedImage;
        anchors.fill: parent;
        visible: !sunken();
    }

    Image {
        id: pressedImage;
        anchors.fill: parent;
        visible: sunken();
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent;
    }

    ButtonModel {
        id: model;

        checkable: true;
        // autoExclusive: true;

        pressed: mouseArea.pressed;
        highlighted: mouseArea.containsMouse;
    }

    Script {
        function sunken() {
            return model.down || model.checked;
        }
    }
}
