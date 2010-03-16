import Qt 4.6

Row {
    Repeater {
        model: 6

        Button {
            pressedSource: "images/bt_circle_close_pressed.png";
            releasedSource: "images/bt_circle_close.png";
        }
    }
}
