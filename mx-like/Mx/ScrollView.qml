import Qt 4.7

Item {
    id: root;

    property alias contentX: container.x;
    property alias contentY: container.y;
    property alias contentWidth: container.width;
    property alias contentHeight: container.height;
    default property alias data: container.data;

    width: 50;
    height: 50;

    Item {
        id: clipper;
        clip: true;

        anchors.top: parent.top;
        anchors.bottom: bottomSlider.top;
        anchors.left: parent.left;
        anchors.right: rightSlider.left;

        Item {
            id: container;

            function fitsCompletely()
            {
                // Are scrollbars needed at all?
                //
                // This method avoids the chicken-and-egg problem that happens
                // due to the fact that adding scrollbars effectively reduce
                // the viewable area.
                // Thus we check whether we could live without bars at all and
                // then, in a second step, we check whether we need horizontal
                // and/or vertical bars.
                return (width <= root.width) && (height <= root.height);
            }
        }
    }

    ScrollBar {
        id: rightSlider;

        anchors.left: parent.right;
        anchors.top: parent.top;
        anchors.bottom: bottomSlider.top;
        visible: false;
        vertical: true;

        states: [
            State {
                name: "shown";
                when: !container.fitsCompletely() && (container.height > clipper.height);
                PropertyChanges {
                    target: container;
                    y: -rightSlider.value * (container.height - clipper.height);
                }
                PropertyChanges {
                    target: rightSlider;
                    anchors.leftMargin: -rightSlider.width;
                    visible: true;
                }
            }
        ]
    }

    ScrollBar {
        id: bottomSlider;

        anchors.top: parent.bottom;
        anchors.left: parent.left;
        anchors.right: rightSlider.left;
        visible: false;

        states: [
            State {
                name: "shown";
                when: !container.fitsCompletely() && (container.width > clipper.width);
                PropertyChanges {
                    target: container;
                    x: -bottomSlider.value * (container.width - clipper.width);
                }
                PropertyChanges {
                    target: bottomSlider;
                    anchors.topMargin: -bottomSlider.height;
                    visible: true;
                }
            }
        ]
    }
}