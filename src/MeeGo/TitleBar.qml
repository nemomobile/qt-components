import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: titlebar;
    anchors.left: parent.left;
    anchors.right: parent.right;
    z: 1000;
    property alias showBackButton : escapeButton.back;

    property alias dropShadowHeight: dropShadow.height;
    signal minimize();
    signal quit();
    signal backClicked();

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MNavigationBarStyle"
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    ScalableImage {
        id: dropShadow
        anchors.top: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        style: meegostyle
        imageProperty: "dropShadowImage"
    }

    IconButton {
        id: home;
        iconId: "icon-m-framework-home"
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;

        onClicked: { titlebar.minimize(); }
    }
    IconButton {
        id: escapeButton;

        property bool back: false;
        iconId: back ? "icon-m-framework-back" : "icon-m-framework-close"
        anchors.right: parent.right;
        anchors.verticalCenter: parent.verticalCenter;

        onClicked: { back ? titlebar.backClicked() : titlebar.quit(); }
    }
}
