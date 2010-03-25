import Qt 4.6
import "common"
import "toolbar"
import "titlebar"
import "checkbox"
import "lineedit"
import "radiobutton"
import "pushbutton"
import "scrollbar"
import "slider"
import "ratingslider"

Rectangle {

    width: 480
    height: 800

    FontLoader { id: droidSans; source: "fonts/Droid-Sans/DroidSans.ttf" }
    FontLoader { id: droidSansBold; source: "fonts/Droid-Sans/DroidSans-Bold.ttf" }

    TitleBar {
        x:0; y: 0; width: parent.width; height: 60
        z: 1
        title: "Fancybar"
        Text {
            x: 0
            y: 61
 text: "titlebar";
        anchors.bottomMargin: -78
 anchors.bottom: parent.top }
    }

    CheckBox {
        x:0; y: 192
        Text {
            x: 0
            y: -18
 text: "checkbox";
        anchors.bottomMargin: 1
 anchors.bottom: parent.top }
    }

    LineEdit {
        x:90; y: 183
        Text { text: "lineedit"; anchors.bottom: parent.top }
    }

    Column {
        x:220; y: 81
        width: 120
        property var selectedButton

        RadioButton {
            x: 0
            y: 0
 width: parent.width; height: 30; text: "radio 1" }
        RadioButton { width: parent.width; height: 30; text: "radio gaga" }
    }

    PushButton {
        x:0; y: 86
        Text {
            x: 0
            y: 51
 text: "pushbutton";
        anchors.bottomMargin: -68
 anchors.bottom: parent.top }
    }

    Text {
        x: 0
        y: 235
 text: "slider";
    anchors.bottomMargin: 2
    anchors.leftMargin: 0
 anchors.left: basicSlider.left; anchors.bottom: basicSlider.top }
    MiniSlider {
        id: basicSlider
        x:0; y: 254
    }

    Text { text: "volumeslider"; anchors.bottom: volumeSlider.top; anchors.left: volumeSlider.left  }
    VolumeSlider {
        id: volumeSlider
        x:1; y: 333
    }

    Text { text: "slider"; anchors.bottom: ratingSlider.top; anchors.left: ratingSlider.left }
    RatingSlider {
        id: ratingSlider
        x:1; y: 406
    }

    Text { text: "scrollbar"; anchors.bottom: dummyList.top; anchors.left: dummyList.left }
    DummyListView {
        id: dummyList
        width: parent.width
        anchors.top: ratingSlider.bottom
        anchors.bottom: toolBar.top
        x: 1; y:473
        clip: true
        ScrollBar {
            width: 11
            height: dummyList.height
            scrollArea: dummyList
            anchors.right: dummyList.right
        }
    }

    ToolBar {
        id: toolBar
        width: parent.width
        anchors.bottom: parent.bottom

        Text { text: "ToolBar"; anchors.bottom:parent.top }
        Row {
            id: toolButtons
            y: parent.topMargin
            ToolButton { }
            ToolButton { }
            ToolButton { }
            ToolButton { }
        }
    }
}
