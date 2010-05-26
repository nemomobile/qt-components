import Qt 4.7
import Components 1.0

Item {
    id: root
    property alias speed: rangeModel.value;

    width: 210; height: 210

    Image { source: "background.png" }

    Image {
        x: 98
        y: 98
        source: "needle_shadow.png"
        transform: Rotation {
            origin.x: 9; origin.y: 0
            angle: needleRotation.angle
        }
    }

    Image {
        id: needle
        x: 98;
        y: 98;
        smooth: true
        source: "needle.png"

        transform: Rotation {
            id: needleRotation
            origin.x: 4; origin.y: 0
            SpringFollow on angle {
                spring: 1.4
                damping: .15
                to: rangeModel.position;
            }
        }
    }

    Image { x: 21; y: 18; source: "overlay.png"; }

    MouseArea {
        id: area;
        anchors.fill: parent;
        anchors.leftMargin: 10;
        anchors.rightMargin: 19;
        anchors.topMargin: 8;
        anchors.bottomMargin: 22;

        // ### This is faster than binding angleModel "x" and "y" properties
        //     because it generates only calculation of angle/radius each time
        //     mouse moves X and Y at once.
        onPositionChanged: angleModel.setXY(mouseX, mouseY);
    }

    Text {
        id: text;
        anchors.top: parent.top;
        anchors.left: parent.left;
        text: "<b>" + Math.round(rangeModel.value) + "</b>";
    }

    RangeModel {
        id: rangeModel;
        maximumValue: 120;
        positionAtMinimum: 49;
        positionAtMaximum: 314;
        position: angleModel.angle;
    }

    AngleModel {
        id: angleModel;
        originX: area.width / 2;
        originY: area.height / 2;
        rotation: 90;

        // ### This works but is slower. It will cause "angle" to be
        //     calculated twice everytime the mouse moves X and Y at the
        //     same time.
        // x: area.mouseX;
        // y: area.mouseY;
    }
}
