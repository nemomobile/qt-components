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

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleType: Style.NavigationBar
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

    TitleButton {
        id: home;
        icon: "icon-m-framework-home"
        anchors.left: parent.left;
        anchors.verticalCenter: parent.verticalCenter;

        onClicked: { titlebar.minimize(); }
    }
    TitleButton {
        id: escapeButton;

        property bool back: false;
        icon: back ? "icon-m-framework-back" :"icon-m-framework-close"
        anchors.right: parent.right;
        anchors.verticalCenter: parent.verticalCenter;

        onClicked: { titlebar.quit(); }
    }
}
