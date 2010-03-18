import Qt 4.6
import Components 1.0

Item {
    id: container
    width: 400;
    height: 150;

    Text {
        id: text;
        anchors.horizontalCenter: parent.center;
        text: "Using as initial date: " + Qt.formatDateTime(model.dateTime, "dd/MM/yyyy - hh:mm:ss");
        color: "black";
    }
    Text {
        id: text2;
        anchors.horizontalCenter: parent.center;
        anchors.top: text.bottom;
        text: "First day of Week: " + Qt.formatDate(model.firstDayOfWeek, "dd/MM/yyyy");
        color: "black";
     }
    MouseArea {
        anchors.fill: parent;
        onClicked: {
            var current = model.weekNumber;
            model.weekNumber = current + 27;
        }
    }
    DateTimeModel {
        id: model
        dateTime: "2010-03-17T10:15:16";
    }
}
