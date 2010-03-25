import Qt 4.6
import "components"
import "../slider"

Item{
    id: container;
    width:200
    height: 50
    property var bubbleTarget: 0;
    property var rating: 0;

    Row{
        id: row
        anchors.fill:parent
        Repeater {
            model:5
            Star {
                id:star
            }
        }
    }
    Bubble {
        id: bubble
        anchors.bottom:parent.top
        anchors.bottomMargin:8
        opacity: mouseRegion.pressed ? 1: 0
        SpringFollow on x { source:bubbleTarget; spring: 10.0; damping: 0.9 }
    }

    function handlePress(x) {
        rating = Math.floor(5*((1+x)/width));
        var i
        for(i = 0 ; i< 5; ++i) {
            if (i == rating) {
                row.children[i].state = "Pressed";
            } else if (i <= rating) {
                row.children[i].state = "Selected";
            } else {
               row.children[i].state = "";
            }
            if (x < 0)
                bubble.label = "None"
            else{
                switch (rating) {
                    case 0 : bubble.label  = "Crap";
                    break;
                    case 1 : bubble.label = "Poor";
                    break;
                    case 2 : bubble.label = "Average";
                    break;
                    case 3 : bubble.label = "Good";
                    break;
                    case 4 :
                    default:
                        bubble.label = "Exellent";
                    // fall through
                    break;
                }
            }
        }
        var itemcenter = row.children[rating].x + row.children[rating].width/2;
        if (itemcenter>0)
            bubbleTarget =  Math.min(width, Math.max(0, itemcenter - bubble.width/2));

    }

    function handleRelease(x) {
        rating = Math.floor(5*((1+x)/width));
        row.children[rating].state = "Selected";
    }

    MouseArea {
        id:mouseRegion
        anchors.fill:parent;
        onReleased: {
            container.handleRelease(mouseX);
            bubble.state = ""
        }
        onPressed: {
            bubble.state = "Pressed"
        }
        onPositionChanged: {
            container.handlePress(mouseX);
        }
    }
}



