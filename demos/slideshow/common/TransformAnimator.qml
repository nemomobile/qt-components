import Qt 4.6

Item {
    id: root;
    width: 800;
    height: 480;
    property string currentImage;
    property string nextImage;
    property bool running: animation.running;

    signal finished();

    function start()
    {
        animation.start();
    }

    Item {
        id: leftContainer;
        width: root.width;
        height: root.height;
        clip: true;

        Image {
            id: leftImage;
            x: -parent.x; // Absolute 0
            width: root.width;
            height: root.height;

            asynchronous: false;
//            smooth: true;
            source: currentImage;
        }
    }

    Item {
        id: rightContainer;
        width: root.width;
        height: root.height;
        anchors.left: leftContainer.right;
        anchors.leftMargin: 10;
        clip: true;

        Image {
            id: rightImage;
            x: -parent.x; // Absolute 0
            width: root.width;
            height: root.height;

            asynchronous: false;
//            smooth: true;
            source: nextImage
        }
    }

    SequentialAnimation {
        id: animation;
        PropertyAnimation {
            target: leftContainer;
            property: "x";
            to: -(leftContainer.width + rightContainer.anchors.leftMargin);
            duration: 900;
        }
        ScriptAction {
            script: resetContainerPosition();
        }
    }

    function resetContainerPosition() {
        currentImage = nextImage;
        leftContainer.x = 0;
        nextImage = "";
        root.finished();
    }
}
