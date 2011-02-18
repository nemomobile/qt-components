import Qt 4.7
import com.nokia.symbian 1.0

Item {
    id: testItem

    width: 200
    height: 400

    // This is used to test default properties
    TabBarLayout {
        id: testLayout
        x: 10
        y: 130
        objectName: "testLayout"
    }

    TabBarLayout {
        id: tabLayout1
        x: 10
        y: 130
        objectName: "tabLayout1"
        Item { id: button1; objectName: "button1" }
        Item { id: button2; objectName: "button2" }
        Item { id: button3; objectName: "button3" }
    }

    TabBarLayout {
        id: tabLayout2
        x: 10
        y: 130
        objectName: "tabLayout2"
        TabButton { id: tabButton1; objectName: "tabButton1" }
        TabButton { id: tabButton2; objectName: "tabButton2" }
    }
}











