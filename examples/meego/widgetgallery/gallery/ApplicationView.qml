import Qt 4.7
import com.meego 1.0

Page {
    title: "Application View"
    ListView {
        anchors.fill: parent
        model: ApplicationViewSections {}
        delegate: BasicListItem {
            title: name
            onClicked: {
                window.nextPage(Qt.createComponent(source));
            }
        }
    }
}
