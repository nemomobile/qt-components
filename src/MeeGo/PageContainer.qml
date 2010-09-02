import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Item {
    id: pageContainer

    property variant page: null
    property real pageY: 0
    property bool __animationEnabled: false

    // checking parent to avoid warning, see QTBUG-13170
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    // can't use Binding element because it emits a warning for assigning 'null' to target
    onPageChanged: { if (page != null) page.y = pageY }
    onPageYChanged: { if (page != null) page.y = pageY }

    states: [
        State {
            name: "left"
            PropertyChanges {
                target: pageContainer
                x: -pageContainer.width
            }
        },
        State {
            name: "right"
            PropertyChanges {
                target: pageContainer
                x: pageContainer.width
            }
        }
    ]

    Style {
        id: meegoPageStyle
        styleClass: "MApplicationPageStyle"
    }
    Background {
        // we need to fill the background for the whole window size in meegotouch
        anchors.fill: parent
        style: meegoPageStyle
    }

    Behavior on x {
        id: slideAnimation
        enabled: __animationEnabled
        SequentialAnimation {
            ScriptAction {
                script:
                    if (state == "")
                        pageContainer.visible = true
            }
            NumberAnimation {}
            ScriptAction {
                script:
                    if (state == "right")
                        pageContainer.destroy(100)
                    else if (state == "left")
                        pageContainer.visible = false
            }
        }
    }
}
