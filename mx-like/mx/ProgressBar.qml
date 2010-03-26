import Qt 4.6

Item {
    id: basicProgressBar;
    property int value;

    width: 160;
    height: 22;

    BorderImage {
        id: background;
        source: "images/progress-bar-background.png";
        anchors.fill: parent;
        border.top:5;
        border.bottom:5;
        border.left:5;
        border.right:5;
    }

    Rectangle {
        id: progressbar;
        x: 3;

        function handleWidth(x) {
            if (x < 0) {
                return 0;
            } else if (x > 100) {
                return parent.width - 6;
            } else {
                return value * (parent.width - 6) / 100;
            }
        }

        width: handleWidth(value);
        color: '#84dbf5';

        anchors.top: background.top;
        anchors.topMargin: 3;
        anchors.bottom: background.bottom;
        anchors.bottomMargin: 3;
    }

    Rectangle {
        id: infinitebar;
        visible: false;
        width: parent.width * 0.2;
        color: '#84dbf5';

        anchors.top: background.top;
        anchors.topMargin: 3;
        anchors.bottom: background.bottom;
        anchors.bottomMargin: 3;

        SequentialAnimation on x {
            loops: Animation.Infinite;
            NumberAnimation {
                to: 279 - infinitebar.width;
                duration: 2000;
            }
            NumberAnimation {
                to: 0;
                duration: 2000;
            }
        }
    }


    states: [
        State {
            name: "still";
            PropertyChanges {
                target: progressbar;
                visible: true;
            }
            PropertyChanges {
                target: infinitebar;
                visible: false;
            }
        },
        State {
            name: "infinite";
            PropertyChanges {
                target: progressbar;
                visible: false;
            }
            PropertyChanges {
                target: infinitebar;
                visible: true;
            }
        }
    ]
}