import Qt 4.7

Item {
    id: root;
    width: 300;
    height: 200;
    property string currentImage;
    property string nextImage;
    property bool running: animation.running;

    signal finished();

    function start()
    {
        animation.start();
    }

    Image {
        id: leftImage;
        width: parent.width;
        height: parent.height;

        asynchronous: false;
        smooth: true;
        source: currentImage;
    }

    Image {
        id: rightImage;
        width: parent.width;
        height: parent.height;
        anchors.left: leftImage.right;
        anchors.leftMargin: 10;

        asynchronous: false;
        smooth: true;
        source: nextImage
    }

    SequentialAnimation {
        id: animation;
        PropertyAnimation {
            target: leftImage;
            property: "x";
            to: -root.width - rightImage.anchors.leftMargin;
            duration: 500;
        }
        // ### We think this will give room for the mainloop to run
        // and the last frame be painted (when "x" reaches the 'to'
        // value).
        PauseAnimation {
            duration: 50
        }
        ScriptAction {
            script: resetContainerPosition();
        }
    }

    function resetContainerPosition() {
        currentImage = nextImage;
        leftImage.x = 0;
        nextImage = "";
        root.finished();
    }
}
