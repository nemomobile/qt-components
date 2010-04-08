import Qt 4.7
import Mx 1.0

Item {
    width: 600;
    height: 600;

    ScrollGrid {
        id: scroll
        x: 10;
        y: 10;
        width: 430;
        height: 430;
    }

    Text {
        id: gotoText;
        anchors.top: gotoEntry.top;
        anchors.bottom: gotoEntry.bottom;
        anchors.left: scroll.left;
        text: "Make button visible:";
        verticalAlignment: Text.AlignVCenter;
    }

    Entry {
        id: gotoEntry;
        anchors.top: scroll.bottom;
        anchors.topMargin: 20;
        anchors.left: gotoText.right;
        anchors.leftMargin: 5;
        anchors.right: scroll.right;

        hint: "";
        onEnterPressed: scroll.ensureVisible(parseInt(text) -1);

    }

    Text {
        id: strideText;
        anchors.top: strideEntry.top;
        anchors.bottom: strideEntry.bottom;
        anchors.left: scroll.left;
        text: "Set max-stride:";
        verticalAlignment: Text.AlignVCenter;
    }

    Entry {
        id: strideEntry;
        anchors.top: gotoEntry.bottom;
        anchors.topMargin: 10;
        anchors.left: gotoEntry.left;
        anchors.right: gotoEntry.right;

        hint: "";
        onEnterPressed: scroll.maxstride = parseInt(text);
    }

}