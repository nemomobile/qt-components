import Qt 4.7
import Components 1.0

Item {
    width: 200
    height: 400

    property int value: 0

    Rectangle {
        x: 10
        y: 10
        width: 30
        height: 30
        color: "chocolate"
    }

    StyleButton {
        x: 50
        y: 50
        width: 100
        height: 100

        text: "These"

        onClicked: {
            text = value;
            value += 1;
            console.log("cliquei!!");
        }

        checkable: true;
        autoExclusive: true;
    }

    StyleButton {
        x: 50
        y: 200
        width: 67
        height: 67

        text: "are"

        checkable: true;
        autoExclusive: true;
    }

    StyleButton {
        anchors.bottom: parent.bottom
        height: 67
        anchors.left: parent.left
        anchors.right: parent.right

        text: "buttons"

        checkable: true;
        autoExclusive: true;
    }
}
