import Qt 4.7
import Components 1.0

Item {
    width: 400
    height: 500

    Column {
        x: 50
        y: 50
        spacing: 10

        LineEdit {
            id: edit1
            emptyText: "Favorite color"
            font.family: "Consolas"
            onAccepted: print("your favorite color is " + text)
        }
        LineEdit {
            emptyText: "Favorite editor (secret!)"
            font.family: "Inconsolata"
            password: true
        }
        LineEdit {
            id: edit3
        }

        // For testing...
        Row {
            Rectangle {
                color: "tomato"
                width: 100
                height: 100

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        edit1.font.family = edit1.font.family === "Consolas" ? "terminus" : "Consolas";
                        print(edit1.font.family);
                    }
                }
            }

            Rectangle {
                color: "chocolate"
                width: 100
                height: 100

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        edit3.text = edit3.text === "iii" ? "MMM" : "iii";
                    }
                }
            }


        }
    }
}
