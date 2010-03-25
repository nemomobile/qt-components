import Qt 4.6

Rectangle {
    id: titleBar
    height: 60; width: 360; z: 1
    property string title: "Title"
    property var titlebarModel: TitleBarModel
    signal backClicked
    signal statusClicked

    Image {
        anchors.fill: parent
        source: "images/TitleBar.png"
        Image {
            source: "images/TitleBarShadow.png"
            width: parent.width
            anchors.top: parent.bottom
            anchors.left: parent.left
        }
        Row {
            id: titleBarLeftButton
            anchors.left: parent.left
            height: parent.height
            Item {
                width: 60
                height: parent.height
                Image {
                    id: titleBarStatus
                    source: "images/TitleBarStatus.png"
                    anchors.centerIn: parent
                    MouseArea {
                        id: titleBarStatusRegion
                        anchors.fill: parent
                        onClicked: titleBar.statusClicked()
                    }
                }
                Rectangle {
                    color: "black"
                    opacity: 0.0
                    visible: titleBarStatusRegion.pressed
                    anchors.fill: parent
                }
            }
            Image {
                source: "images/TitleBarDivider.png"
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
            }
        }

        Item {
            id: titleBarTitle
            height: parent.height
            anchors.left: titleBarLeftButton.right
            anchors.right: titleBarRightButton.left
            Rectangle {
                id: titleBarHighlight
                color: "#55FFFFFF"
                border.width: 3
                border.color: "white"
                anchors.fill: parent
                opacity: 0
                anchors.leftMargin: 2
                anchors.bottomMargin: 2
                anchors.rightMargin: 2
                anchors.topMargin: 2
            }
            BorderImage {
                id: titleBarPressed
                anchors.fill: parent
                border.left: 3
                border.bottom: 3
                border.right: 3
                border.top: 3
                source: "images/TitleBarPressed.png"
                opacity: 0
            }
            Text {
                id: titleBarTitleText
                text: title
                height: parent.height
                verticalAlignment: "AlignVCenter"
                color:  titleBarTitleHit.pressed?"black":"white"
                anchors.left: parent.left
                anchors.leftMargin: 11
                anchors.right: parent.right
                font.family: nokiaSansTitleRegular.name
                font.pointSize: 36
            }
            Image {
                id: titleBarTriangle
                source: titleBarTitleHit.pressed?"images/TitleBarTrianglePressed.png":"images/TitleBarTriangle.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
            }
            MouseArea {
                id: titleBarTitleHit
                anchors.fill:parent
                onClicked: { titleBar.state = titleBar.state=="Latched"?"":"Latched" }
            }
        }


        Row {
            id: titleBarRightButton
            anchors.right: parent.right
            height: parent.height
            Image {
                source: "images/TitleBarDivider.png"
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
            }
            Item {
                width: 60
                height: parent.height

                MouseArea {
                    id: titleBarBackRegion
                    anchors.fill: parent
                    onClicked: titleBar.backClicked()
                }
                Image {
                    id: titleBarBack
                    source: "images/TitleBarBackArrow.png"
                    anchors.centerIn: parent

                }
                Rectangle {
                    color: "black"
                    opacity: 0.0
                    visible: titleBarBackRegion.pressed
                    anchors.fill: parent
                }
            }
        }

    }
    ListView {
        id: titleBarMenu
        opacity: 0
        anchors.horizontalCenter: parent.horizontalCenter
        y: -20
        z: -1
        width: titleBar.width*0.68333 //=246
        height: 165
        orientation: ListView.Vertical
        // effect: DropShadow {
        //     blurRadius: 7
        //     color: "#22555555"
        //     yOffset: 5
        //     xOffset: 5
        // }
        delegate: Component {
            Rectangle {
                width: 246
                height: 55
                border.width: 1
                border.color: "grey"
                color: (menuElementHit.pressed?"#1984c3":"white")
                MouseArea {
                    id: menuElementHit
                    anchors.fill: parent
                }
                Text {
                    text: action
                    font.pointSize: 24
                    font.family: nokiaSansS60Regular.name
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: 15
                }
            }
        }
        model:  titlebarModel
    }


    states: [
        State {
            name: "Highlighted";
            PropertyChanges {
                target: titleBarHighlight
                opacity: 1
            }
            },
            State {
                name: "Latched";
                PropertyChanges {
                    target: titleBarMenu
                    opacity: 1
                }
                PropertyChanges {
                    target: titleBarPressed
                    opacity: 1
                }
                PropertyChanges {
                    target: titleBarTitleText
                    color: "black"
                }
                PropertyChanges {
                    target: titleBarTriangle
                    source: "images/TitleBarTrianglePressed.png"
                }
                PropertyChanges {
                    target: titleBarMenu
                    y: 60
                    opacity: 1
                }
            }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "Latched"
            NumberAnimation {
                targets: titleBarMenu
                properties: "y,opacity"
                duration: 250
                easing.type: "OutQuad"
            }
        }
    ]

}
