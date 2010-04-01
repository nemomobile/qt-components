import Qt 4.7
import Mx 1.0

Item {
    id: scrollview;

    width: 400;
    height: 400;

    property bool vertical: true;
    property int maxstride: 3;
    property int cellWidth: 90;
    property int cellHeight: 35;
    property alias count: grid.count;

    function ensureVisible(index)
    {
        grid.positionViewAtIndex(index, GridView.Contain);
    }

    GridView {
        id: grid;
        width: rightBar.x;
        height: bottomBar.y;
        cellWidth: parent.cellWidth;
        cellHeight: parent.cellHeight;
        overShoot: false;
        //        interactive: false;
        clip: true;
        contentX: bottomBar.value * (contentWidth - width);
        contentY: rightBar.value * (contentHeight - height);

        delegate: Button {
            width: scrollview.cellWidth;
            height: scrollview.cellHeight;
            text: "Button " + (index + 1);
            tooltipText: "test";
            onClicked: { scrollview.vertical = !scrollview.vertical }
        }

        model: 200;
    }

    ScrollBar {
        id: rightBar;
        anchors.top: parent.top;
        anchors.bottom: parent.bottom;
        anchors.left: parent.right;

        vertical: true;
        visible: false;
    }

    ScrollBar {
        id: bottomBar;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: parent.bottom;

        visible: false;
    }

    states: [
        State {
            name: "horizontal";
            when: !scrollview.vertical;
            PropertyChanges {
                target: grid;
                height: Math.min(bottomBar.y, scrollview.maxstride * scrollview.cellHeight);
                flow: GridView.TopToBottom;
            }
            PropertyChanges {
                target: bottomBar;
                anchors.topMargin: -height;
                visible: true;
            }
        },
        State {
            name: "vertical";
            when: scrollview.vertical;
            PropertyChanges {
                target: grid;
                width: Math.min(rightBar.x, scrollview.maxstride * scrollview.cellWidth);
                flow: GridView.LeftToRight;
            }
            PropertyChanges {
                target: rightBar;
                anchors.leftMargin: -width;
                visible: true;
            }
        }
    ]
}
