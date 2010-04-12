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
            emptyText: "Favorite color"
            font.family: "Consolas"
            onAccepted: print("your favorite color is " + text)
        }
        LineEdit {
            emptyText: "Favorite editor (secret!)"
            font.family: "Inconsolata"
            password: true
        }
        LineEdit {}
    }
}
