import Qt 4.7

Item {
    id: scrollbar
    property string orientation: "horizontal" //###

    height: 30
    width: 200

    signal scrollStart
    signal scrollStop

    Rectangle  {
        id: scrollbarPath

        height: 28

        anchors.verticalCenter: parent.verticalCenter

        anchors.left: leftButton.right
        anchors.right: rightButton.left

        color: '#dee2e5'

        MouseArea {
            function handleRelease(x) {
                handle.x = (x + handle.width <= scrollbarPath.width ? x : x - handle.width)
            }
            anchors.fill: parent
            onPressed: { handleRelease(mouseX);}
        }

        Image {
            id: handle
            source: "images/scroll-"
                + (scrollbar.orientation == "horizontal" ? "hhandle" : "vhandle")
                + (handleMouseRegion.containsMouse ? "-hover" : "") + ".png"
            width: 64 //###

            MouseArea {
                id: handleMouseRegion
                anchors.fill: parent
                hoverEnabled: true
                drag.target: handle
                drag.axis: "XAxis"
                drag.minimumX: 0
                drag.maximumX: scrollbarPath.width - width
                onPressed: { console.log(handle.x) }
            }
        }
    }

    BorderImage {
        id: leftButton

        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        source: "images/scroll-button-"
            + (scrollbar.orientation == "horizontal" ? "left" : "up")
            + (leftButtonMouseRegion.containsMouse ? "-hover" : "") + ".png"

        MouseArea {
            id: leftButtonMouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                console.log(handle.x);
                if (handle.x >= 10)
                    handle.x = handle.x - 10; // ###
            }
        }
    }

    BorderImage {
        id: rightButton

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        source: "images/scroll-button-"
            + (scrollbar.orientation == "horizontal" ? "right" : "down")
            + (rightButtonMouseRegion.containsMouse ? "-hover" : "") + ".png"

        MouseArea {
            id: rightButtonMouseRegion
            hoverEnabled: true
            anchors.fill: parent
            onClicked: {
                console.log(handle.x);
                if (handle.x + handle.width <= scrollbarPath.width - 10)
                    handle.x = handle.x + 10;  //###
            }
        }
    }
}
