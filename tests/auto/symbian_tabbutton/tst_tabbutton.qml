import Qt 4.7
import com.nokia.symbian 1.0

Item {
    id: testItem

    width: 200
    height: 400

    // This button is used to test different button properties
    TabButton {
        id: testButton
        x: 10
        y: 130
        objectName: "testButton"
    }

    TabGroup {
        id: tabGroup
        objectName: "tabGroup"
        Item { id: tab1 }
        Item { id: tab2 }
    }

    TabButton {
        id: tabButton1
        objectName: "tabButton1"
        tab: tab1
    }

    TabButton {
        id: tabButton2
        objectName: "tabButton2"
        tab: tab2
    }

}











