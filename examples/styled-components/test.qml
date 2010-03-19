import Qt 4.7
import Components 1.0

Item {
    width: 200
    height: 400

    Rectangle {
        x: 10
        y: 10
        width: 30
        height: 30
        color: "chocolate"
    }

    Botao {
        x: 50
        y: 50
        width: 100
        height: 100
    }

    Botao {
        x: 50
        y: 200
        width: 67
        height: 67
    }

    Botao {
        anchors.bottom: parent.bottom
        height: 67
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
