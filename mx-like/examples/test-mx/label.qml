import Qt 4.7
import Mx 1.0

Item {
    // ### Original example loaded a css file.

    // ### embed this in label?
    Rectangle {
        anchors.fill: label
        color: "yellow"
    }

    Label {
        id: label
        x: 50
        y: 50

        text: "Hello World!"
        color: "red"
        font.pixelSize: 30
        font.family: "Helvetica"
    }
}
