import QtQuick 1.0
import com.nokia.symbian 1.0
import com.nokia.extras 1.0

Page {
    id: container

    Row {
        anchors.centerIn: parent

        spacing: platformStyle.paddingLarge * 4

        Button {
            text: "Show"
            onClicked: banner.open();
        }

        Button {
            text: "Hide"
            onClicked: banner.close();
        }
    }

    InfoBanner {
        id: banner

        text: "Information to be shown to user"
        iconSource: "assets/info_banner_thumbnail.png"
        timeout: 4000
        interactive: true
    }

    Component.onCompleted: banner.open()
}
