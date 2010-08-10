import Qt 4.7
import Qt.labs.components 1.0

/// This is an example of a button component with 'checkable' abilities, it can
/// be a two-state toggle or part of a exclusive group. The contents of this
/// example is in an unusual order to emphasize the role of each component in
/// the tree.

Item {
    id: root

    width: 100
    height: 30

    // Our button has a background, which the color depends on whether it is
    // 'pressed' or 'checked'.
    Rectangle {
        id: background
        anchors.fill: root

        // Due to QML property binding, if either 'pressed' or 'checked' changes
        // then the color is recalculated.
        color: {
            if (pressed)
                return "crimson";
            if (checked)
                return "gold";
            return "tomato";
        }

    }

    // This inner rectangle complements the background, the lighter color and
    // smaller size makes the other rectangle looks like a border.
    Rectangle {
        color: { Qt.lighter(background.color); }
        anchors.fill: root
        anchors.margins: 2
    }


    // The first new property our button exposes is the 'text'. Since it is
    // only used to set the caption of our button with an Text QML item, we can
    // just alias the property.
    //
    // For users of this button, there shouldn't be a difference between using a
    // common property and an alias property, this is an implementation detail.
    property alias text: label.text

    Text {
        id: label
        anchors.fill: root
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }


    // Users of our button would like to know whether it is pressed, and also
    // have the convenience of being signalled when it was clicked. So we do
    // that by having a new property and signal.
    property alias pressed: area.pressed
    signal clicked

    // To implement it, we use a MouseArea QML item, which is the regular way
    // of tracking mouse events.
    MouseArea {
        id: area
        anchors.fill: root

        // When the mouse area is clicked, we want to emit the 'clicked' signal
        // of our button (root) and also tell the Checkable component that it
        // needs to toggle itself (more details in the next comment).
        onClicked: {
            ch.toggle();
            root.clicked();
        }
    }


    // Since this button can be used as a toggle button, we also have a
    // property to indicate this condition, and whether it is checked or
    // not. In both cases we "delegate" those by aliasing them to the Checkable
    // component.
    property alias checkable: ch.enabled
    property alias checked: ch.checked

    // The Checkable component acts as a helper to deal with the
    // checked/unchecked state and also with mutually exclusive groups of
    // checkable items (CheckableGroups).
    //
    // It has one slot (used earlier in the onClicked handler) to indicate that
    // the button needs to be toggled. Whether or not this will change the
    // state of other buttons in the group is something that Checkable will
    // take care of.
    Checkable {
        id: ch

        // CheckableGroups have a property to indicate which value is checked
        // (only one can be since it is mutually exclusive). This property sets
        // the value when this button is checked.
        value: root.text

        // By default the button will act as a regular button. Remeber the
        // button 'checkable' is aliased to this property, so when the user
        // marks "checkable: true" it will enable it.
        enabled: false
    }

    // This makes possible to users of the button set the exclusivity group
    // that the button will be part of. Again this information internally is
    // just delegated to the Checkable.
    property alias exclusiveGroup: ch.exclusiveGroup
}
