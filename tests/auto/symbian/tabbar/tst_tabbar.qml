import QtQuick 1.0
import com.nokia.symbian 1.0

Item {
    id: testItem

    width: 200
    height: 400

    // This is used to test default properties
    TabBar {
        id: testTabBar
        x: 10
        y: 130
        objectName: "testTabBar"
    }

    TabBar {
        id: tabBar1
        x: 10
        y: 130
        objectName: "tabBar1"
        Item { id: button1; objectName: "button1" }
        Item { id: button2; objectName: "button2" }
        Item { id: button3; objectName: "button3" }
    }
}











