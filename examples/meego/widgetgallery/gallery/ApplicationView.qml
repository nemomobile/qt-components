import Qt 4.7
import com.meego 1.0

Page {
    tools: commonTools
    ListView {
        anchors.fill: parent
        model: ApplicationViewSections {}
        delegate: BasicListItem {
            title: name
            onClicked: {
                pageStack.push(Qt.createComponent(source));
            }
        }
    }
}
