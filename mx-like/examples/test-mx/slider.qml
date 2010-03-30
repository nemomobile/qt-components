import Qt 4.7
import Mx 1.0

Item {

    // ### set initial value to 0
    Slider {
        id: slider
        x: 20
        y: 20
        width: 280
    }

    Rectangle {
        x: 108
        y: 52
        width: 64
        height: 64
        color: "red"

        // For simple things, property bindings are helpful
        opacity: slider.value
    }
}
