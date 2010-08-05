import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Background {
    id: titlebar;
    anchors.left: parent.left;
    anchors.right: parent.right;
    z: 1000;
    property alias showBackButton : escapeButton.back;

    property alias dropShadowHeight: dropShadow.height;
    signal minimize();
    signal quit();

    style: meegostyle;

    Component.onCompleted: {
        // XXX This is not nice
        // What I wanted to do was to bind the root item "implicitSize" to the
        // size provided by meegostyle. However:
        //  - Implicit size is not exported to QML, that's C++ accessible only
        //  - Binding actual width and height to the sizes provided by meegostyle
        //    is not OK. It can cause sizes set externally to be overriden if
        //    the binding is re-evaluated.
        width = meegostyle.preferredWidth
        height = meegostyle.preferredHeight
    }

    Style {
        id: meegostyle
        styleType: Style.NavigationBar
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
