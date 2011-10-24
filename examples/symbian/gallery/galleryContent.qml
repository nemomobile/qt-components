import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: galleryContent

    Flickable {
        id: flickable

        anchors.fill: parent
        clip: true
        contentHeight: column.height

        SampleColumn {
            id: column

            enabled: galleryContent.parent ? galleryContent.parent.columnEnabled : true
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
