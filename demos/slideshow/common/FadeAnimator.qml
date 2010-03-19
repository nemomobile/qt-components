import Qt 4.6

Item {
    id: root;
    width: 300;
    height: 200;
    property string currentImage;
    property string nextImage;
    signal finished();

    function start()
    {
        animation.start();
    }

    Image {
        id: oldImage;
        anchors.fill: parent;

        asynchronous: false;
        source: currentImage;
    }

    Image {
        id: newImage;
        anchors.fill: parent;
        opacity: 0;

        asynchronous: true;
        source: nextImage
    }

    SequentialAnimation {
        id: animation;

        ParallelAnimation {
            PropertyAnimation {
                target: oldImage;
                property: "opacity";
                to: 0
                duration: 1000;
                easing.type: "OutCubic"
            }
            PropertyAnimation {
                target: newImage;
                property: "opacity";
                to: 1
                duration: 1000;
                easing.type: "InCubic"
            }
        }
        ScriptAction {
            script: resetContainerPosition();
        }
    }

    Script {
        function resetContainerPosition() {
            currentImage = nextImage;
            oldImage.opacity = 1;
            newImage.opacity = 0;
            nextImage = "";
            root.finished();
        }
    }
}
