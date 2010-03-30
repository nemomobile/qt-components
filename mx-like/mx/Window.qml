import Qt 4.7

Item{//This should be ROOT element in a QML/MX app
    property int headerBottom: header.height //Children should have at least this y
    WindowModel{}
    Rectangle{
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
        }
        Image{
            anchors.right:parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            source: 'images/close' + (closeMA.containsMouse?'-hover':'') + '.png'
            MouseArea{ id: closeMA
                anchors.fill: parent
                hoverEnabled:true
                onClicked: Qt.quit()
            }
        }
    }
}
