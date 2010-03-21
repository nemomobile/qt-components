import Qt 4.6

//Picture View
Item {
    id: root;
    anchors.fill: parent;

    property alias interval: timer.interval;
    property alias running: timer.running;
    property alias animator: loader.source;
    property alias animItem: loader.item;

    property int index: 0;

    // Triggers the animator transitions
    // default interval: 1000 milliseconds
    Timer {
        id: timer;
        repeat: true;
        onTriggered: {
            animItem.start();
        }
    }

    // view animator loader
    Loader {
        id: loader;
        anchors.fill: parent;

        onStatusChanged: {
            switch (loader.status) {
                case Loader.Ready: {
                    animItem.anchors.fill = loader;
                    init();
                    break;
                }
                case Loader.Null:
                case Loader.Error: {
                    console.log('Error: Cannot load view animator');
                    loader.source =  "";
                    timer.running = false;
                    break;
                }
            }
        }
    }

    Script {
        function musicas() {
            //### s/hardcoded stuff/model/
            var list = ["/home/anselmo/Pictures/Oslo/DSC07202.JPG",
                           "/home/anselmo/Pictures/Oslo/DSC07203.JPG",
                           "/home/anselmo/Pictures/Oslo/DSC07204.JPG",
                           "/home/anselmo/Pictures/Oslo/DSC07205.JPG",
                           "/home/anselmo/Pictures/Oslo/DSC07207.JPG"
                           ];

             return list;
        }

        function init()
        {
            var list = musicas();
            animItem.currentImage = list[0];
            animItem.nextImage = list[1];
            index = 1;
            animItem.finished.connect(getNext);
        }

        function getNext()
        {
            var list = musicas();
            index = (index + 1) % 5; //### photos list size
            animItem.nextImage = list[index];
        }
    }
}
