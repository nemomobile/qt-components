import QtQuick 1.0
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

Item {
    id: root
    anchors.fill: parent
    property string replaceMode: "set"
    visible: true
    Component.onCompleted: {
        stack.push(firstPage)
    }

    PageStack {
        id: stack
        anchors.fill: parent
        clip: true
    }

    Page {
        id: firstPage
        tools: bar1
        ToolBar {
            id: bar1
            anchors.bottom: parent.bottom
            //make sure the content of the page does not hide the toolbar
            z: firstPage.z+1
        }
        Column {
            id: columni
            Row {
                width: firstPage.width
                CheckBox {
                    id: showBackButton
                    checked: true
                    text: "back"
                }
                CheckBox {
                    id: flatter
                    checked: false
                    text: "flat"
                }

                Button {
                    text: "Portrait"
                    onClicked: {
                        startupOrientationButton.orientation = Screen.Portrait
                        screen.allowedOrientation = Screen.Portrait
                    }
                }
                Button {
                    text: "Landscape"
                    onClicked: {
                        startupOrientationButton.orientation = Screen.Landscape
                        screen.allowedOrientation = Screen.Landscape
                    }
                }
            }
            Row {
                CheckableGroup { id: group1 }
                Row {
                    RadioButton {
                        id: rb1
                        platformExclusiveGroup: group1
                        text: "set"
                        onClicked: root.replaceMode = "set"
                    }
                    RadioButton {
                        id: rb2
                        platformExclusiveGroup: group1
                        text: "push"
                        onClicked: root.replaceMode = "push"
                    }
                    RadioButton {
                        id: rb3
                        platformExclusiveGroup: group1
                        text: "pop"
                        onClicked: root.replaceMode = "pop"
                    }
                    RadioButton {
                        id: rb4
                        platformExclusiveGroup: group1
                        text: "replace"
                        onClicked: root.replaceMode = "replace"
                    }
                }
            }
            Row {
                Button { text: "B"; onClicked: bar1.setTools(layout, root.replaceMode) }
                Button { text: "B+M"; onClicked: bar1.setTools(layout1, root.replaceMode) }
                Button { text: "B+T+M "; onClicked: bar1.setTools(layout2, root.replaceMode) }
                Button { text: "B+2T+M"; onClicked: bar1.setTools(layout3, root.replaceMode) }
            }
            Row {
                Button { text: "B+2T"; onClicked: bar1.setTools(layout4, root.replaceMode) }
                Button { text: "2xT+M"; onClicked: bar1.setTools(layout5, root.replaceMode) }
                Button { text: "B+I+M"; onClicked: bar1.setTools(layout6, root.replaceMode) }
            }
            Row {
                Button { text: "B+2I+M"; onClicked: bar1.setTools(layout7, root.replaceMode) }
                Button { text: "B+3I+M"; onClicked: bar1.setTools(layout8, root.replaceMode) }
                Button { text: "mix"; onClicked: bar1.setTools(layout11, root.replaceMode) }
                Button { text: "mix2"; onClicked: bar1.setTools(layout12, root.replaceMode) }
            }
            Row {
                Button { text: "TabButtons in ButtonRow"; onClicked: stack.push(buttonRowPage) }
            }
        }
    }

    Page {
        Column {
            Text {
                text: "Page2\n click next to go to \nthird page and back to return to first"
                color: "white"
            }
        }
        id: secondPage
        tools: bar2
        ToolBar {
            id: bar2
            anchors.bottom: parent.bottom
            tools: ToolBarLayout {
                backButton: true
                ToolButton {
                    flat: flatter.checked
                    iconSource: "qrc:tb_back.svg"
                    onClicked: stack.pop()
                }
                ToolButton {
                    flat: flatter.checked
                    text: "next"
                    onClicked: stack.push(thirdPage)
                }
            }
        }
    }

    Page {
        id: thirdPage
        Column {
            Text {
                text: "Page3\n no toolbar"
                color: "white"
            }
            Button {
                text: "back"
                onClicked: stack.pop();
            }
        }
    }

    Page {
        id: buttonRowPage
        tools: buttonRowToolBar

        Text {
            text: "Current tab: " + tabGroup.currentTab.objectName
            color: "red"
        }

        TabGroup {
            id: tabGroup
            anchors { bottom: buttonRowToolBar.top; top: parent.top; left: parent.left; right: parent.right }

            Item {
                id: tab1
                objectName: "tab1"
                anchors.fill: parent
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    width: 60
                    height: 60
                }
            }

            Item {
                id: tab2
                objectName: "tab2"
                anchors.fill: parent
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    width: 120
                    height: 120
                }
            }

            Item {
                id: tab3
                objectName: "tab3"
                anchors.fill: parent
                BusyIndicator {
                    anchors.centerIn: parent
                    running: true
                    width: 240
                    height: 240
                }
            }
        }

        ToolBar {
            id: buttonRowToolBar
            anchors.bottom: parent.bottom
            //make sure the content of the page does not hide the toolbar
            z: buttonRowPage.z+1
            tools: ToolBarLayout {
                backButton: true
                ToolButton {
                    id: backButton
                    iconSource: "qrc:tb_back.svg"
                    onClicked: stack.pop()
                }
                ButtonRow {
                    anchors { top: parent.top; bottom: parent.bottom; left: backButton.right; right: parent.right }

                    TabButton { id: tabButton1; tab: tab1; text: "Tab1"; }
                    TabButton { id: tabButton2; tab: tab2; text: "Tab2"; }
                    TabButton { id: tabButton3; tab: tab3; text: "Tab3"; }
                }
            }
        }
    }

    ToolBarLayout {
        visible: false
        backButton: showBackButton.checked
        id: layout
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
    }

    ToolBarLayout {
        id: layout1
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout2
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            text: "button"
        }

        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout3
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            text: "click"
        }
        ToolButton {
            text: "click"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout4
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            text: "click"
        }
        ToolButton {
            text: "click"
        }
    }

    ToolBarLayout {
        id: layout5
        backButton: showBackButton.checked
        ToolButton {
            text: "click"
        }
        ToolButton {
            text: "click"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout6
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout7
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout8
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout9
        backButton: showBackButton.checked

        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout10
        backButton: showBackButton.checked
        ToolButton {
            flat: parent.backButton
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            flat: flatter.checked
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout11
        backButton: showBackButton.checked
        Button {
            text: "Button"
        }
        Button {
            text: "Button"
        }
        Button {
            text: "Button"
        }
    }

    ToolBarLayout {
        id: layout12
        backButton: showBackButton.checked

        ToolButton {
            flat: flatter.checked
            iconSource: "qrc:tb_plus.png"
        }
        Button {
            text: "Text"
        }
        Button {
            text: "Text"
        }
        Button {
            text: "Text"
        }
        Button {
            text: "Text"
        }
    }
}
