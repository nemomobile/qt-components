import Qt 4.7

Item {
    id: combobox;

    property int elementsToShow;
    property alias current: value.text;
    property alias model: elements.model;
    property alias currentIndex: elements.currentIndex;

    width: 161;
    height: 42;
    elementsToShow: 5;

    BorderImage {
        id: background;
        source: "images/combobox-toolbar.png";
        anchors.fill: parent;
        border.left:5;
        border.right:5;
    }

    Text {
        id: value;
        color: 'white'
        anchors.verticalCenter: parent.verticalCenter;
        anchors.left: background.left;
        anchors.right: marker.left;
        anchors.leftMargin: 5;
        font.pixelSize: 15
    }

    BorderImage {
        id: marker;
        source: "images/combobox-toolbar-marker.png";
        border.left:5;
        border.right:5;
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
                    x: 11;
                    y: 11;
                    spacing: 11;
                    Text {
                        text: content;
                        color: currentIndex==index?'#44b6e8':'white'
                       font.pixelSize: 15
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

        Rectangle {
            id: listBackground;
            border.color: "#dadada";
            color: '#434343'
            radius: 5
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
