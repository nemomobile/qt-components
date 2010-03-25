import Qt 4.6

BorderImage {
    id: base ;
    property var scrollArea

    source: 'images/ScrollbarBase.png'
    border.left: 4; border.right: 4; border.top: 4; border.bottom: 4
    width: 11

    parent: scrollArea
    anchors.right: scrollArea.right


    onScrollAreaChanged: {
        parent = scrollArea;
        anchors.right = parent.right
        scrollArea: parent
    }

    BorderImage {
        id: scrollBarThumb
        source: 'images/ScrollbarThumbNonInteractive.png'
        border.left: 2; border.right: 2; border.top: 2; border.bottom: 2
        anchors.horizontalCenter: parent.horizontalCenter
        width: 8
        y: {
            var ny = scrollArea != undefined ? scrollArea.visibleArea.yPosition * base.height : 0;
            if (ny > 4) return ny; else return 4;
        }
        height: {
            var nh = scrollArea != undefined ? scrollArea.visibleArea.heightRatio * base.height : 0;
            var ny = scrollArea != undefined ? scrollArea.visibleArea.yPosition * base.height : 0;
            if (ny > 5) {
                var t = scrollArea.height - 3 - ny;
                if (nh > t) return t; else return nh;
            } else return nh + ny;
        }
    }
    MouseArea {
        id: thumbRegion
        anchors.fill: parent
        hoverEnabled: true
    }

    states: [
    State {
        name: 'Pressed'; when: thumbRegion.pressed
        PropertyChanges { target: scrollBarThumb; source: 'images/ScrollbarThumbPressed.png' }
    },
    State {
        name: 'Hover'; when: thumbRegion.containsMouse
        PropertyChanges { target: scrollBarThumb; source: 'images/ScrollbarThumbActive.png' }
    }
    ]
}
