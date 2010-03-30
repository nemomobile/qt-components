import Qt 4.7
import Mx 1.0

Item {
    // ### Long press support
    Button {
        x: 100
        y: 50

        text: "Normal Button"
    }

    Button {
        x: 100
        y: 100

        text: "Toggle Button"
        checkable: true
    }
}
