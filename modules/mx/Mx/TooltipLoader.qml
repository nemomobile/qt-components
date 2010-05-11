import Qt 4.7

MyLoader {
    id: tooltipLoader;

    property string text: "";
    property bool shown: false;
    loaded: text != "";

    delegate: Tooltip {
        id: tooltipDelegate;

        anchors.top: parent.bottom;
        anchors.horizontalCenter: parent.horizontalCenter;

        text: tooltip.text;
        shown: tooltip.shown;
    }
}
