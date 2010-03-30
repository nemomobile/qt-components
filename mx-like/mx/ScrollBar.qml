import Qt 4.7

Item {
    id: scrollbar

    signal scrollStart
    signal scrollStop
    state: "horizontal"
    property real value: handle.x / (scrollbarPath.width - handle.width)

    height: 50
    width: 50

    Rectangle  {
        id: scrollbarPath

        color: '#dee2e5'

        MouseArea {
            anchors.fill: parent
            onPressed: {
                handleRelease(mouseX, mouseY);
                scrollbar.scrollStart();
            }
            onReleased: {
                scrollbar.scrollStop();
            }

            //###
            function handleRelease(x, y) {
                if (scrollbar.state == "horizontal")
                    handle.x = (x + handle.width <= scrollbarPath.width ? x : x - handle.width)
                else
                    handle.y = (y + handle.height <= scrollbarPath.height ? y : y - handle.height)
            }
        }

        BorderImage {
            id: handle

            MouseArea {
                id: handleMouseRegion
                anchors.fill: parent
                hoverEnabled: true
                drag.target: handle

                onPressed: { scrollbar.scrollStart(); }
                onReleased: { scrollbar.scrollStop(); }
            }
        }
    }

    Image {
        id: button1

        source: "images/scroll-button-"
            + (scrollbar.state == "horizontal" ? "left" : "up")
            + (button1MouseRegion.containsMouse ? "-hover" : "") + ".png"

        MouseArea {
            id: button1MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                if (scrollbar.state == "horizontal") {
                    if (handle.x >= 10)
                        handle.x = handle.x - 10; // ### step size
                } else {
                    if (handle.y >= 10)
                        handle.y = handle.y - 10; // ### step size
                }
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }
        }
    }

    Image {
        id: button2

        source: "images/scroll-button-"
            + (scrollbar.state == "horizontal" ? "right" : "down")
            + (button2MouseRegion.containsMouse ? "-hover" : "") + ".png"

        MouseArea {
            id: button2MouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                if (scrollbar.state == "horizontal") {
                    if (handle.x + handle.width <= scrollbarPath.width - 10)
                        handle.x = handle.x + 10;  //### step size
                } else {
                    if (handle.y + handle.height <= scrollbarPath.height - 10)
                        handle.y = handle.y + 10;  //### step size
                }
                scrollbar.scrollStart();
                scrollbar.scrollStop();
            }
        }
    }

    states: [
        State {
            name: "horizontal"
            AnchorChanges {
                target: scrollbarPath
                left: button1.right
                right: button2.left
            }
            AnchorChanges {
                target: button1
                left: scrollbar.left
            }
            AnchorChanges {
                target: button2
                right: scrollbar.right
            }
            PropertyChanges {
                target: scrollbarPath
                height: button1.height
            }
            PropertyChanges {
                target: handleMouseRegion
                drag.axis: "XAxis"
                drag.minimumX: 0
                drag.maximumX: scrollbarPath.width - handle.width
            }

            PropertyChanges {
                target: handle

                border.left: 10
                border.right: 10

                source: "images/scroll-hhandle"
                + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png"
            }
        },
        State {
            name: "vertical"
            AnchorChanges {
                target: scrollbarPath
                top: button1.bottom
                bottom: button2.top
            }
            AnchorChanges {
                target: button1
                top: scrollbar.top
            }
            AnchorChanges {
                target: button2
                bottom: scrollbar.bottom
            }
            PropertyChanges {
                target: scrollbarPath
                width: button1.width
            }
            PropertyChanges {
                target: handleMouseRegion
                drag.axis: "YAxis"
                drag.minimumY: 0
                drag.maximumY: scrollbarPath.height - handle.height
            }
            PropertyChanges {
                target: handle

                border.top: 10
                border.bottom: 10

                source: "images/scroll-vhandle"
                + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png"
            }
        }
    ]

}
