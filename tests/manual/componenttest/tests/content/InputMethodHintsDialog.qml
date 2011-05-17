import QtQuick 1.0
import com.nokia.symbian 1.0
import Qt.labs.components 1.0

Dialog {
    id: selectInputMethodHints
    height: parent.height; width: parent.width
    property variant inputMethodHintsFlag : 0

    title: Text {
        text: "InputMethodHints"
        anchors.fill: parent
        color: "white"
        font { pointSize: 16; bold: true }
        horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
    }

    buttons: Button {
                text: "Close"
                width: parent.width
                onClicked: selectInputMethodHints.accept()
            }

    content: ListView {
                id: listView
                height: parent.height; width: parent.width
                clip: true
                model: InputMethodHints { }
                delegate: textFieldDelegate
                section.property: "category"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate
                ScrollBar {
                    flickableItem: listView
                    anchors { top: listView.top; right: listView.right }
                }
            }

    Component {
        id:textFieldDelegate

        ListItem {
            id: listItem
            height: listView.height / 7; width: listView.width

            ListItemText {
                id: listText
                role: "SubTitle"; text: title
                anchors {
                    left: listItem.paddingItem.left; right:checkBox.left
                    verticalCenter: parent.verticalCenter
                }
            }

            CheckBox {
                id: checkBox
                checked: selected
                anchors { right:listItem.paddingItem.right; verticalCenter: parent.verticalCenter }
                onClicked: {
                    inputMethodHintsFlag ^= flag
                    listView.model.set(index, { "selected": checkBox.checked })
                }
            }

            onClicked: {
                inputMethodHintsFlag ^= flag
                checkBox.checked = !checkBox.checked
                listView.model.set(index, { "selected": checkBox.checked })
            }
        }
    }

    Component {
        id: sectionDelegate

        ListHeading {
            width: listView.width
            id: sectionHeader

            ListItemText {
                anchors.fill: sectionHeader.padding
                role: "Heading"; text: section
            }
        }
    }
}
