import Qt 4.7
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
        longPress: true
        objectName: "testButton"
    }

    Button {
        id: button2
        objectName: "button2"
    }

    Button {
        id: button3
        text: "test"
        iconSource: "image://theme/qtg_small_selected"
        x: 10
        y: 180
        width: 70
        height: 40
        objectName: "button3"
    }

    Button {
        id: button4
        text: "test"
        iconSource: "image://theme/qtg_small_unselected"
        x: 10
        y: 230
        width: 70
        height: 40
        objectName: "button4"
    }

    Button {
        id: button5
        iconSource: "image://theme/qtg_small_unselected"
        x: 10
        y: 280
        width: 70
        height: 30
        objectName: "button5"
    }
}











