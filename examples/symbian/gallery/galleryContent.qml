import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: galleryContent

    anchors.fill: parent

    Flickable {
        id: flickable

        anchors.fill: parent
        clip: true
        contentHeight: column.height

        SampleColumn {
            id: column

            enabled: galleryContent.parent.columnEnabled
            anchors {
                left: parent.left
                right: parent.right
                margins: column.spacing
            }

            childrenInverted: root.childrenInverted
            windowInverted: root.platformInverted
        }
    }

    ScrollDecorator {
        flickableItem: flickable
        platformInverted: root.childrenInverted
    }
}
