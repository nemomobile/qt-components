import Qt 4.7
import Mx 1.0

Window {
    width: 800
    height: 600

    Row {
        x: 0
        y: parent.headerBottom
        width: parent.width
        height: parent.height - y

        ButtonGroup {
            id: group
            width: col.width
            height: col.height

            // ### in original, vbox arranges the widths to be the
            // same, column here is just a positioner. Workaround for
            // now: we have a smallest button size big enough...
            Column {
                id: col

                Button { text: "Label"; checkable: true }
                Button { text: "Entry"; checkable: true }
                Button { text: "Button"; checkable: true }
                Button { text: "Button group"; checkable: true }
                Button { text: "Combo Box"; checkable: true }
                Button { text: "Progress Bar"; checkable: true }
                Button { text: "Slider"; checkable: true }
                Button { text: "Toggle"; checkable: true }
                Button { text: "Tooltips"; checkable: true }
                Button { text: "Expander"; checkable: true }
                Button { text: "Scroll Grid"; checkable: true }
                Button { text: "Scroll Bar"; checkable: true }
                Button { text: "Scroll View"; checkable: true }
                Button { text: "Styles"; checkable: true }
            }

            onCheckedItemChanged: {
                loader.source = "test-mx/" + checkedItem.text.toLowerCase().replace(" ", "-") + ".qml";
            }
        }

        Loader {
            id: loader

            anchors.left: group.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            opacity: 1.0

            onStatusChanged: {
                if (loader.status === Loader.Ready) {
                    loader.item.anchors.fill = loader;
                }
            }
        }

        Item {
            id: notImplemented
            anchors.fill: loader
            opacity: 0.0

            Text {
                x: 40
                y: 40
                text: "Not implemented yet."
                font.pixelSize: 20
            }
        }
    }

    states: [
        State {
            name: "not"
            when: { loader.status === Loader.Error; }
            PropertyChanges {
                target: notImplemented
                opacity: 1.0
            }
            PropertyChanges {
                target: loader
                opacity: 0.0
            }
        }
    ]
}
