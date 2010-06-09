import Qt 4.7

Rectangle {
    width: 800; height: 480
    Row {
        Repeater {
            model: 6

            Button {
                pressedSource: "images/bt_circle_close_pressed.png";
                releasedSource: "images/bt_circle_close.png";
            }
        }
    }
}
