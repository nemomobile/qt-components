import Qt 4.6


//Picture View
Rectangle {
    id: root
    anchors.fill: parent
    color: "black"

    property alias source: loader.source
    property int speed

    state: "paused"

    // view loader
    Loader {
        id: loader
        anchors.fill: parent
        clip: true

        onStatusChanged: {
            switch (loader.status) {
                case Loader.Ready: {
                    console.log('Loaded');
                    break;
                }
                case Loader.Loading: {
                    console.log('Loading');
                    break;
                }
                case Loader.Error: {
                    console.log('Error');
                    loader.source =  ""
                    break;
                }
                case Loader.Null: {
                    console.log('Null')
                }
            }
        }
    }
}
