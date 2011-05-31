import QtQuick 1.0
import com.nokia.symbian 1.0

Item {
    id: testItem

    width: 200
    height: 400

    // This button is used to test different button properties
    Button {
        id: testButton
        text: "test"
        x: 10
        y: 130
        width: 70
        height: 40
        objectName: "testButton"
    }

    Button {
        id: button2
        objectName: "button2"
    }

    Button {
        id: button3
        text: "test"
        iconSource: "image://theme/qtg_graf_radiobutton_normal_selected"
        x: 10
        y: 180
        width: 70
        height: 40
        objectName: "button3"
    }

    Button {
        id: button4
        text: "test"
        iconSource: "image://theme/qtg_graf_radiobutton_normal_unselected"
        x: 10
        y: 230
        width: 70
        height: 40
        objectName: "button4"
    }

    Button {
        id: button5
        iconSource: "image://theme/qtg_graf_radiobutton_normal_unselected"
        x: 10
        y: 280
        width: 70
        height: 30
        objectName: "button5"
    }

    Button {
        id: buttonFont1
        objectName: "buttonFont1"
        font.family: "Arial"
    }

    Button {
        id: buttonFont2
        objectName: "buttonFont2"
        font.pixelSize: 55
    }
}











