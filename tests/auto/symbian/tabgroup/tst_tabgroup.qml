import QtQuick 1.0
import com.nokia.symbian 1.0

Item {
    id: testItem

    width: 200
    height: 400

    // This tab group is used to test different button properties
    TabGroup {
        id: testGroup
        x: 10
        y: 130
        objectName: "testGroup"
    }

    TabGroup {
        id: tabGroup1
        objectName: "tabGroup1"
        Item { id: group1tab1; objectName: "group1tab1" }
        Item { id: group1tab2; objectName: "group1tab2"
        }
    }

    TabGroup {
        id: tabGroup2
        objectName: "tabGroup2"

        Component.onCompleted: {
            var tab1 = tabPageComponent.createObject(tabGroup2)
            tab1.objectName = "group2tab1"

            var tab2 = tabPageComponent.createObject(tabGroup2)
            tab2.objectName = "group2tab2"

            currentTab = tab2
        }
    }

    TabGroup {
        id: tabGroup3
        objectName: "tabGroup3"
        currentTab: group3tab2
        Item { id: group3tab1; objectName: "group3tab1" }
        Item { id: group3tab2; objectName: "group3tab2" }
    }

    Component {
        id: tabPageComponent
        Page {
        }
    }
}











