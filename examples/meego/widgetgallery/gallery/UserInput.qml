import QtQuick 1.0
import com.meego 1.0

Page {
    id: page
    anchors.fill: parent
    tools: commonTools

    ListModel {
        id: m

        ListElement {
            name: "Text Input"
            source: "TextInput.qml"
        }

        ListElement {
            name: "Slider"
            source: "Slider.qml"
        }
    }

    ListView {
        anchors.fill: parent
        model: m

        delegate: BasicListItem {
            title: name
            onClicked: {
                pageStack.push(Qt.createComponent(source));
            }
        }
    }
}
