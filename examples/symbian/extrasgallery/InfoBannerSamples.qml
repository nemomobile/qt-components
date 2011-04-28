import Qt 4.7
import com.nokia.symbian 1.0
import com.nokia.extras 1.0

Page {
    id: infoBannerSamples

    Flickable {
        anchors { left: parent.left; right: parent.right; leftMargin: platformStyle.paddingLarge; rightMargin: platformStyle.paddingLarge }
        height: parent.height
        contentHeight: 600

        Column {
            id: col1
            spacing: platformStyle.paddingLarge
            Text {
                id: label
                color: platformStyle.colorNormalLight
                text: "Click on SHOW button to display example InfoBanners"
                font.bold: true
            }

            Button {
                id: button
                text: "Show"
                onClicked: {
                    banner1.open();
                    banner2.open();
                    banner3.open();
                    banner4.open();
                    banner5.open();
                    banner6.open();
                }
            }
        }

        InfoBanner{
            id: banner1
            objectName: "infoBanner1Object"
            text: "List title #3 lorem ipum dolor."
            anchors { top: col1.bottom; topMargin: platformStyle.paddingLarge }
        }

        InfoBanner{
            id: banner2
            text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in."
            anchors { top: banner1.bottom; topMargin: platformStyle.paddingLarge }
        }

        InfoBanner{
            id: banner3
            text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in at metus erat, a sodales ipsum dolor sit."
            anchors { top: banner2.bottom; topMargin: platformStyle.paddingLarge }
        }

        InfoBanner{
            id: banner4
            objectName: "infoBanner2Object"
            text: "List title #3 lorem ipum dolor."
            iconSource: "assets/info_banner_thumbnail.png"
            anchors { top: banner3.bottom; topMargin: platformStyle.paddingLarge }
        }

        InfoBanner{
            id: banner5
            text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in."
            iconSource: "assets/info_banner_thumbnail.png"
            anchors { top: banner4.bottom; topMargin: platformStyle.paddingLarge }
        }

        InfoBanner{
            id: banner6
            text: "List title #3 lorem ipum dolor sit amet, consectetur adipiscing in at metus erat, a sodales."
            iconSource: "assets/info_banner_thumbnail.png"
            anchors { top:banner5.bottom; topMargin: platformStyle.paddingLarge }
        }
    }
}

