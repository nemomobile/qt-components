import Qt 4.7
import Components 1.0

Column {
    id: root;
    width: 50;

    spacing: 10;

    Rectangle {
        id: rect;
        color: "tomato";

        width: parent.width;
        height: width;

        Rectangle {
            id: dot;

            width: 2 * radius;
            height: 2 * radius;
            radius: 5;

            x: angleModel.x - radius;
            y: angleModel.y - radius;

            color: "black";
        }

        AngleModel {
            id: angleModel;
            originX: rect.width / 2;
            originY: rect.height / 2;
            radius: radiusRange.value;
            angle: angleRange.value;
        }
    }

    Rectangle {
        id: radiusKnob;

        width: 20;
        height: 20;
        radius: 5;

        color: "blue";

        MouseArea {
            id: radiusArea;
            anchors.fill: parent;

            drag.axis: "XAxis";
            drag.maximumX: root.width - parent.width;
            drag.target: parent;
        }

        RangeModel {
            id: radiusRange;

            positionAtMaximum: radiusArea.drag.maximumX;
            position: radiusKnob.x;
            maximumValue: (rect.width / 2) - (dot.width / 2);
        }
    }

    Rectangle {
        id: angleKnob;

        width: 20;
        height: 20;
        radius: 5;

        color: "blue";

        MouseArea {
            id: angleArea;
            anchors.fill: parent;

            drag.axis: "XAxis";
            drag.maximumX: root.width - parent.width;
            drag.target: parent;
        }

        RangeModel {
            id: angleRange;

            positionAtMaximum: angleArea.drag.maximumX;
            position: angleKnob.x;
            minimumValue: -720
            maximumValue: 1440;
        }
    }
}