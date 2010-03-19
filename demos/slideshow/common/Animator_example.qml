import Qt 4.6

Item {
    width: 640;
    height: 480;

    FadeAnimator {
        id: anim1;

        anchors.fill: parent;
        onFinished: getNext();
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: anim1.start();
    }

    Item {
        property int index: 0;

        Component.onCompleted: init();

        Script {

            function musicas() {
                var musicas = ["/home/fleury/Downloads/ancora.jpg",
                               "/home/fleury/Desktop/2975263571_a8ef7f0aec_o.jpg",
                               "/home/fleury/Desktop/ComprovanteUNICAMP34.jpg",
                               "/home/fleury/Downloads/akademy-eduardo.jpg"]
                return musicas;
            }

            function init()
            {
                var list = musicas();
                anim1.currentImage = list[0];
                anim1.nextImage = list[1];
                index = 1;
            }

            function getNext()
            {
                var list = musicas();
                index = (index + 1) % 4;
                anim1.nextImage = list[index];
            }
        }
    }
}