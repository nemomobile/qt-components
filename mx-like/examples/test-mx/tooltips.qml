import Qt 4.7
import Mx 1.0

Item {
    Button {
        x: 50
        y: 100
        text: "Hide Me"
        tooltipText: "Disappear"

        onClicked: { visible = false; }
    }

    // ### was a MxFrame...
    Row {
        x: 50
        y: 200
        width: child.width
        height: child.height

        Button {
            id: child
            text: "Testing 123"
            tooltipText: "Testing tooltips in a container..."
        }
    }

    // ### Tooltip should be inside the app screen... (small offset here is needed)
    Button {
        x: 50
        y: 300
        text: "Testing Long Text"
        tooltipText: "Here is some really long text to test the handling in MxTooltip"
    }

    Button {
        x: 360
        y: 300
        text: "Testing Long Text"
        tooltipText: "Here is some really long text to test the handling in MxTooltip"
    }

    // ### Eliding support (and knowing the screen size)
    Button {
        x: 250
        y: 5
        text: "Crazy"
        tooltipText: ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" +
                      " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" +
                      " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" +
                      " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
    }

    Button {
        x: 250
        y: 430
        text: "Bottom"
        tooltipText: "Hello Hello Hello"
    }
}
