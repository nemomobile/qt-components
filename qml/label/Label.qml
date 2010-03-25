import Qt 4.6
Text {
        FontLoader { id: droidSans; source: "../fonts/Droid-Sans/DroidSans.ttf" }
        FontLoader { id: droidSansBold; source: "../fonts/Droid-Sans/DroidSans-Bold.ttf" }
        id:text
        font.family: droidSans.name
        font.pointSize: 18
}
