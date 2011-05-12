import QtQuick 1.0
import com.nokia.symbian 1.0

Rectangle {
    id: background
    width: screen.width
    height: 20
    color: "black"

    ProgressBar {
        id: memoryUsageBar
        anchors.fill: parent
        minimumValue: 0
        maximumValue: 67108864 // 64 MB
        value: symbian.privateAllocatedMemory()

        Text {
            id: memText
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: memTimer.running ? "white" : "gray"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: memTimer.running ? memTimer.stop() : memTimer.start()
        }

        Timer {
            id: memTimer
            repeat: true
            running: memoryUsageBar.visible
            interval: 500

            onTriggered: {
                memoryUsageBar.value = symbian.privateAllocatedMemory()
                memText.text = symbian.privateAllocatedMemory()+"/"+memoryUsageBar.maximumValue
            }
        }
    }
}


