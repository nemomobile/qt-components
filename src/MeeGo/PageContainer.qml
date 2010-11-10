import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Item {
    id: pageContainer

    property Item page: null
    property alias topDecorationHeight: topMarginBinding.value
    property alias bottomDecorationHeight: bottomMarginBinding.value
    property bool __animationEnabled: false

    // checking parent to avoid warning, see QTBUG-13170
    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    Binding {
        id: topMarginBinding
        target: page
        property: "anchors.topMargin"
        value: topDecorationHeight
    }

    Binding {
        id: bottomMarginBinding
        target: page
        property: "anchors.bottomMargin"
        value: bottomDecorationHeight
    }

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
                script: {
                    if (state == "right" || state == "left")
                        page.aboutToExit()
                    else if (state == "")
                        pageContainer.visible = true
                }
            }
            NumberAnimation {}
            ScriptAction {
                script: {
                    if (state == "right")
                        pageContainer.destroy(100)
                    else if (state == "left")
                        pageContainer.visible = false
                    else
                        page.entered()
                }
            }
        }
    }
}
