import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

import "ButtonGroup.js" as Private

Row {
    id: root

    property string groupType: "One" // or "Many" or "None"
    property variant checkedButton: group.selectedValue

    CheckableGroup {
        id: group
    }

    function __onChildrenChanged() {
        Private.updateButtons(children);
    }

    function __onGroupTypeChanged() {
        // This way any other unexpectable value is equivalent to "None"
        var checkable = (groupType === "One" || groupType === "Many");
        var exclusive = (groupType === "One");
        Private.setParameters(checkable, exclusive, group, Qt.Horizontal);
    }

    Component.onCompleted: {
        __onGroupTypeChanged();
        __onChildrenChanged();

        root.childrenChanged.connect(__onChildrenChanged);
        root.groupTypeChanged.connect(__onGroupTypeChanged);
    }

    Component.onDestruction: {
        root.childrenChanged.disconnect(__onChildrenChanged);
        root.groupTypeChanged.disconnect(__onGroupTypeChanged);
    }
}
