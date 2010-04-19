import Qt 4.7

MyLoader {
    id: tooltipLoader;

    property string text: "";
    property bool shown: false;
    loaded: text != "";

    delegate: Tooltip {
        id: tooltipDelegate;

        // ### This will trigger "unable to assign undefined value" errors
        // because 'parent' is not defined upon the creation of this item :-/
        // anchors.top: parent.bottom;
        // anchors.horizontalCenter: parent.horizontalCenter;

        text: tooltip.text;
        shown: tooltip.shown;

        // ### This is a hack to avoid the error mentioned above
        states: State {
            name: "withParent";
            when: { parent.toString() !== "null" }
            AnchorChanges {
                target: tooltipDelegate;
                anchors.top: parent.bottom;
                anchors.horizontalCenter: parent.horizontalCenter;
            }
        }
    }
}
