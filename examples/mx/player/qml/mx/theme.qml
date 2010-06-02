import Qt 4.7
import Mx 1.0

Rectangle {
    width: 500
    height: 130
    color: "white"

    Label {
        id: songLabel
        anchors.top: parent.top
        anchors.left: parent.left
        text: view.title
    }

    Label {
        id: artistLabel
        anchors.top: songLabel.bottom
        anchors.left: parent.left
        text: view.artist
    }

    Slider {
        id: volumeSlider
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: artistLabel.right
        anchors.leftMargin: 200
        anchors.right: parent.right

        value: view.volume
    }

    ProgressBar {
        id: progressbar
        anchors.top: artistLabel.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        value: view.currentTime / view.totalTime
    }

    Row {
        id: row
        spacing: 15
        anchors.top: progressbar.bottom
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Button {
            id: previous
            text: "previous"
            onClicked: view.previousClicked()
        }
        Button {
            id: playpause
            text: (view.state == 3 ? "pause" : "play")
            onClicked: {
                if (view.state == 3)
                    view.pauseClicked()
                else
                    view.playClicked()
            }
        }
        Button {
            id: stop
            text: "stop"
            onClicked: view.stopClicked()
        }
        Button {
            id: next
            text: "next"
            onClicked: view.nextClicked()
        }
    }
}
