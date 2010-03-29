import Qt 4.7

Item {
    id: combobox;

    property int elementsToShow;
    property alias current: value.text;
    property alias model: elements.model;
    property alias currentIndex: elements.currentIndex;

    width: 161;
    height: 27;
    elementsToShow: 5;

    BorderImage {
        id: background;
        source: "images/combobox.png";
        anchors.fill: parent;
        border.top:5;
        border.bottom:5;
        border.left:5;
        border.right:5;
    }

    Text {
        id: value;
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: background.left;
        anchors.right: marker.left;
        anchors.leftMargin: 5;
    }

    BorderImage {
        id: marker;
        source: "images/combobox-marker.png";
        anchors.verticalCenter: background.verticalCenter;
        anchors.right: background.right;
        anchors.rightMargin: 10;
    }

    MouseArea {
        id: markerArea;
        anchors.fill: background;
        onClicked: {
            list.state == "" ? list.state = "shown" : list.state = "";
            elements.currentIndex = list.lastIndex;
        }
    }

    Item {
        id: list;
        property int lastIndex;

        opacity: 0;
        height: Math.min(background.height * elements.count,
                         background.height * combobox.elementsToShow);

        anchors.top: background.bottom;
        anchors.left: background.left;
        anchors.right: background.right;

        Component {
            id: delegate
            Item {
                id: wrapper
                width: background.width;
                height: background.height;

                Row {
                    x: 5;
                    y: 5;
                    Text {
                        text: content;
                    }
                    Image {
                        source: icon;
                        anchors.verticalCenter: parent.verticalCenter;
                        height: wrapper.height - 10;
                    }
                }

                function selectItem(index) {
                    combobox.current = elements.model.get(index).content;
                    list.lastIndex = index;
                    list.state = "";
                }

                MouseArea {
                    anchors.fill: parent;
                    hoverEnabled: true;

                    onEntered: {
                        elements.currentIndex = index;
                    }
                    onClicked: selectItem(index);
                }

                Keys.onPressed: {
                    if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
                        selectItem(index);
                    } else if (event.key == Qt.Key_Escape) {
                        list.state = "";
                    }
                }
            }
        }

        Component {
            id: highlight
            Rectangle {
                color: "#84dbf5";
            }
        }


        Rectangle {
            id: listBackground;
            border.color: "#8e8e8e";
            anchors.fill: parent;
        }

        ListView {
            id: elements;
            anchors.fill: parent;

            clip:true;
            focus: true;
            overShoot: false;
            keyNavigationWraps: true;

            delegate: delegate;
            highlight: highlight;
        }

        states: [
            State {
                name: "shown";
                PropertyChanges {
                    target: list;
                    opacity: 1;
                }
            }
        ]

        transitions: [
            Transition {
                from: "shown";
                to: "";
                NumberAnimation {
                    target: list;
                    properties: "opacity";
                    easing.type: "OutCubic";
                    duration: 500;
                }
            }
        ]
    }
}