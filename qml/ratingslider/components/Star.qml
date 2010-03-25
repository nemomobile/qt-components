import Qt 4.6
import "Star.qml"

Image {
    id: starbase
    source: "../images/StarNormal.png"
    smooth:true
    scale: 1
    transformOrigin: Item.Center
    Image {
        id: ratestar
        opacity:0
        source: "../images/StarNormalRating.png"
        smooth:true
        scale: 1
        transformOrigin: Item.Center
    }
    Image {
        id: pressedstar
        opacity:0
        source: "../images/StarPressed.png"
        smooth:true
        scale: 1
        transformOrigin: Item.Center
    }
    states: [
        State {
            name: "Pressed"
            PropertyChanges { target: starbase; rotation: "72"}
            PropertyChanges { target: pressedstar; opacity: "1"}
            PropertyChanges { target: starbase; scale: "1.1"}
        },
        State {
            name: "Selected"
            PropertyChanges { target: starbase; rotation: "72"}
            PropertyChanges { target: ratestar; opacity: "1"}
            PropertyChanges { target: pressedstar; opacity: "0"}
        }
    ]
    transitions: [
        Transition {
            from: ""; to: "Selected"
            NumberAnimation { properties: "rotation"; duration: 300 }
            NumberAnimation { properties: "opacity"; duration: 200 }
        },
        Transition {
            from: ""; to: "Pressed"
            NumberAnimation { properties: "rotation"; duration: 300 }
            NumberAnimation { properties: "opacity"; duration: 100 }
            NumberAnimation { properties: "scale"; duration: 200 }
        },
        Transition {
            from: "Selected"; to: ""
            NumberAnimation { properties: "rotation"; duration: 300}
            NumberAnimation { properties: "opacity"; duration: 200 }
        },
        Transition {
            from: "Pressed"; to: ""
            NumberAnimation { properties: "rotation"; duration: 300}
            NumberAnimation { properties: "opacity"; duration: 200 }
            NumberAnimation { properties: "scale"; duration: 200 }
        },
        Transition {
            from: "Pressed"; to: "Selected"
            NumberAnimation { properties: "rotation"; duration: 300 }
            NumberAnimation { properties: "opacity"; duration: 200 }
            NumberAnimation { properties: "scale"; duration: 200 }
        }
    ]
}

