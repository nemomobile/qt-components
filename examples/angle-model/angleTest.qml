import Qt 4.7
import Components 1.0

Rectangle {
    id: rect;
    color: "tomato";

    width: 100;
    height: 100;

    MouseArea {
        id: area;
        anchors.fill: parent;
    }

    Rectangle {
        id: dot;

        width: 2 * radius;
        height: 2 * radius;
        radius: 5;

        x: model.x - radius;
        y: model.y - radius;

        color: "black";
    }

    Text {
        text: "Angle: <b>" + Math.round(model.angle) + "</b>";
    }

    Text {
        text: "Radius: <b>" + Math.round(model.radius) + "</b>";
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
    }

    AngleModel {
        id: model;
        originX: rect.width / 2;
        originY: rect.height / 2;
        x: area.mouseX;
        y: area.mouseY;
    }
}