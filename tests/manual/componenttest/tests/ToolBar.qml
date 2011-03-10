import Qt 4.7
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

            Text {
                width:parent.width
                wrapMode:Text.WordWrap
                color: "white"
                text: "backbutton sets the property to current layout.\n"+
                      "If not selected, leftmost space is never occupied\n" +
                      "'+' button pushes page2\n" +
                      "Next Layout button changes the layout in toolbar"
            }

            Row {
                width: firstPage.width
                CheckBox {
                    id: showBackButton
                    checked: true
                    checkable: true
                    text: "back button"
                }
                Button {
                    text: "Portrait"
                    onClicked: {
                        startupOrientationButton.orientation = Screen.Portrait
                        screen.orientation = Screen.Portrait
                   }
                }
                Button {
                    text: "Landscape"
                    onClicked: {
                        startupOrientationButton.orientation = Screen.Landscape
                        screen.orientation = Screen.Landscape
                    }
                }
            }

            Row {
                CheckableGroup { id: group1 }
                Row {
                    RadioButton {
                        id: rb1
                        exclusiveGroup: group1
                        text: "set"
                        onClicked: root.replaceMode = "set"
                    }
                    RadioButton {
                        id: rb2
                        exclusiveGroup: group1
                        text: "push"
                        onClicked: root.replaceMode = "push"
                    }
                    RadioButton {
                        id: rb3
                        exclusiveGroup: group1
                        text: "pop"
                        onClicked: root.replaceMode = "pop"
                    }
                    RadioButton {
                        id: rb4
                        exclusiveGroup: group1
                        text: "replace"
                        onClicked: root.replaceMode = "replace"
                    }
                }
            }

            Row {
                Button {
                    text: "Next Layout"
                    onClicked: {
                        if (bar1.tools == null )
                            bar1.setTools(layout,root.replaceMode)
                        else if (bar1.tools == layout)
                            bar1.setTools(layout2,root.replaceMode)
                        else if (bar1.tools == layout2)
                            bar1.setTools(layout3,root.replaceMode)
                        else if (bar1.tools == layout3)
                            bar1.setTools(layout4,root.replaceMode)
                        else if (bar1.tools == layout4)
                            bar1.setTools(layout5,root.replaceMode)
                        else if (bar1.tools == layout5)
                            bar1.setTools(layout6,root.replaceMode)
                        else if (bar1.tools == layout6)
                            bar1.setTools(layout7,root.replaceMode)
                        else if (bar1.tools == layout7)
                            bar1.setTools(layout8,root.replaceMode)
                        else if (bar1.tools == layout8)
                            bar1.setTools(layout9,root.replaceMode)
                        else if (bar1.tools == layout9)
                            bar1.setTools(layout10,root.replaceMode)
                        else if (bar1.tools == layout10)
                            bar1.setTools(layout11,root.replaceMode)
                        else if (bar1.tools == layout11)
                            bar1.setTools(layout12,root.replaceMode)
                        else if (bar1.tools == layout12)
                            bar1.setTools(layout13,root.replaceMode)
                        else if (bar1.tools == layout13)
                            bar1.setTools(layout,root.replaceMode)
                    }
                }
            }
        }
    }

    Page {
        Text {
            text: "Page2\n click next to go to \nthird page and back to return to first"
            color: "white"
        }
        id: secondPage
        tools: bar2
        ToolBar {
            id: bar2
            anchors.bottom: parent.bottom
            tools: ToolBarLayout {
                backButton: true
                ToolButton {
                    iconSource: "qrc:tb_plus.svg"
                    onClicked: stack.pop()
                }
                ToolButton {
                    text: "Next"
                    onClicked: stack.push(thirdPage)
                }
            }
        }
    }

    Page {
        id: thirdPage
        tools: bar3
        Text {
            text: "Page3\n click back to return to second"
            color: "white"
        }
        ToolBar {
            id: bar3
            anchors.bottom: parent.bottom
            tools: ToolBarLayout {
                backButton: true
                ToolButton {
                    iconSource: "qrc:tb_plus.png"
                    onClicked: stack.pop()
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
            onClicked: stack.pop()
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout2
        backButton: showBackButton.checked
        ToolButton {
            text:"button"
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
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }


    ToolBarLayout {
        id: layout4
        backButton: showBackButton.checked
        ToolButton {
            text: "click"
        }
        ToolButton {
            text: "click"
        }
        ToolButton {
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout5
        backButton: showBackButton.checked
        ToolButton {
            iconSource: "qrc:tb_back.svg"
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
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_back.svg"
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
            text: "click"
        }
        ToolButton {
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout8
        backButton: showBackButton.checked

        ToolButton {
            text: "click"
        }
        ToolButton {
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_back.svg"
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
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
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
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            iconSource: "qrc:tb_back.svg"
        }
        ToolButton {
            onClicked: stack.push(secondPage)
            iconSource: "qrc:tb_plus.png"
        }
    }

    ToolBarLayout {
        id: layout11
        backButton: showBackButton.checked
        Item {
            width: testbutton1.width
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                id: testbutton1
                text: "Button"
            }
        }
        Item {
            width: kekke2.width
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                id: kekke2
                text: "Button"
            }
        }
    }

    ToolBarLayout {
        id: layout12
        backButton: showBackButton.checked

        ToolButton {
            iconSource: "qrc:tb_plus.svg"
        }

        Item {
            width: children[0].width
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                text:"Button"
            }
        }
        Item {
            width: children[0].width
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                text: "Button"
            }
        }
        Item {
            width: children[0].width
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                text: "Button"
            }
        }
        Item {
            width: children[0].widthh
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                text: "Button"
            }
        }
    }

    ToolBarLayout {
        id: layout13
        backButton: showBackButton.checked
        ToolButton {
            iconSource: "qrc:tb_back.svg"
        }

        Item {
            width: children[0].width
            height: parent.height
            Button {
                y: parent.y + parent.height/2 - height/2
                text: "Button 1"
            }
        }

        Button {
            anchors.verticalCenter: parent.verticalCenter
            text: "Button 2"
        }
        Button {
            anchors.verticalCenter: parent.verticalCenter
            text: "Button 3"
        }
        Button {
            anchors.verticalCenter: parent.verticalCenter
            text: "Button 4"
        }
    }
}
