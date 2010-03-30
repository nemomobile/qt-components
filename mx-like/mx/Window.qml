import Qt 4.7

Item{//This should be ROOT element in a QML/MX app
    property int headerBottom: header.height //Children should have at least this y
    WindowModel{id:wm}
    Rectangle{
        id:background
        anchors.fill: parent
        anchors.rightMargin: 1
        anchors.bottomMargin: 1
        color: "white"
        border.color: "black"
    }
    Rectangle{
            z:10
        id:header
        width: parent.width
        height:64
        color:'#333333'
        TitlebarCombobox{id:combo;//API redesign required after I figure out what
                                  //on earth this this is SUPPOSED to do.
            anchors.centerIn: parent
            current: 'Select'
            model: ListModel{
                ListElement{
                    content: 'Hello'
                }
                ListElement{
                    content: 'World'
                }
            }
            z:1
        }
        Image{id:closeImg
            anchors.right:parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            source: 'images/close' + (closeMA.containsMouse?'-hover':'') + '.png'
            MouseArea{ id: closeMA
                anchors.fill: parent
                hoverEnabled:true
                onClicked: Qt.quit()
            }
            z:1
        }
        MouseArea{
            anchors.fill: parent
            onReleased: wm.resetMove();
            onPositionChanged: wm.dragPointMove(mouse.x, mouse.y);
            z:0
        }
    }
    Image{id:resizeImg
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 1
        source: 'images/resize-grip.png'
        MouseArea{ id: resizeMA
            anchors.fill: parent
            onReleased: wm.resetResize();
            onPositionChanged: wm.dragPointResize(mouse.x, mouse.y);
        }
        z:10
    }
}
