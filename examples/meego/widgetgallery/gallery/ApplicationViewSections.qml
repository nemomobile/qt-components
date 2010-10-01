import Qt 4.7

ListModel {
    id:applicationViewSections

    ListElement {
        name: "Application Menu"
        source: "ApplicationMenu.qml"
    }
    ListElement {
        name: "Display Modes"
        source: "DisplayModes.qml"
    }
    ListElement {
        name: "Toolbar (including Tab Bar)"
        source: "ToolbarTabBar.qml"
    }
}
