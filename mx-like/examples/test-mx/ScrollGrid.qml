import Qt 4.7
import Mx 1.0

ScrollView {
    id: scrollview;

    property bool vertical: true;
    property int maxstride: 4;
    property int cellWidth: 90;
    property int cellHeight: 35;
    contentWidth: grid.width;
    contentHeight: grid.height;

    // [0 - count) range
    function ensureVisible(index)
    {
        if (state == "vertical") {
            var row = Math.floor(index / maxstride);
            var atTopY = -row * cellHeight;
            var atBottomY = atTopY + height - cellHeight;
            if (contentY < atTopY) {
                contentY = atTopY;
            } else if (contentY > atBottomY) {
                contentY = atBottomY;
            }
        } else {
            var column = Math.floor(index / maxstride);
            var atLeftX = -column * cellWidth;
            var atRightX = atLeftX + width - cellWidth;
            if (contentX < atLeftX) {
                contentX = atLeftX;
            } else if (contentX > atRightX) {
                contentX = atRightX;
            }
        }
    }

    Flow {
        id: grid;

        property int count: 200;

        Repeater {
            model: grid.count;
            Button {
                width: scrollview.cellWidth;
                height: scrollview.cellHeight;
                text: "Button " + (index + 1);
                tooltipText: "test";
                onClicked: { scrollview.vertical = !scrollview.vertical }
            }
        }
    }

    states: [
        State {
            name: "horizontal";
            when: !scrollview.vertical;
            PropertyChanges {
                target: grid;
                width: Math.ceil(grid.count / scrollview.maxstride) * scrollview.cellWidth;
                height: scrollview.maxstride * scrollview.cellHeight;
                flow: Flow.TopToBottom;
            }
        },
        State {
            name: "vertical";
            when: scrollview.vertical;
            PropertyChanges {
                target: grid;
                width: scrollview.maxstride * scrollview.cellWidth;
                height: Math.ceil(grid.count / scrollview.maxstride) * scrollview.cellHeight;
                flow: Flow.LeftToRight;
            }
        }
    ]
}
