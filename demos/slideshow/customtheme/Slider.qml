import Qt 4.6
import Components 1.0

import "../common/model"

Item {
    id: slider

    property alias value: model.value
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue

    width: 300;
    height: 300

    Image {
        id: background
        source: "images/background.png";

        width: parent.width
        height: parent.height;

        Image {
            id: knob
            x: width / 2 + 2;
            y: parent.height / 2 - height / 2 - 2;

            smooth: true;
            source: "images/needle.png";

            rotation: -40;
            transformOrigin: Item.Right;

            Behavior on rotation {
                NumberAnimation {
                    easing.type: "OutBounce";
                    duration: 500;
                }
            }
        }

        Image {
            id: overlay;
            source: "images/overlay.png";
            x: 15;
            y: 17;
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPositionChanged: {
            var tx;
            if (mouse.x < (parent.width/2)) {
                tx = -((parent.width / 2) - mouse.x);
            } else {
                tx = mouse.x - (parent.width / 2);
            }

            var ty;
            if (mouse.y < (parent.height/2)) {
                ty = (parent.height / 2) - mouse.y;
            } else {
                ty = -(mouse.y - (parent.height / 2));
            }

            var ang = -((Math.atan(ty / tx) * 180 / Math.PI));
            if (mouse.x > parent.width / 2 ||
                (mouse.x > parent.width / 2 && mouse.y > parent.height / 2)) {
                ang = ang + 180;
            }

            if (ang >= -42 && ang <= 222) {
                knob.rotation = ang;
                model.position = ang;
            }
        }
    }


    MyRangeModel {
        id: model
        maximumPosition: 221;
        minimumPosition: -42;
        minimumValue: 0;
        maximumValue: 120;
        position: 0;
    }
}
