import Qt 4.7
import com.meego 1.0

Page {
    title: "Application Menu"

    actions:  [
        Action {
            iconId: "icon-m-toolbar-addressbook"
            text:"Italic"
            onTriggered: {
                applicationMenuText.font.italic = !applicationMenuText.font.italic
            }
        }
    ]

    Text {
        id: applicationMenuText
        anchors.fill: parent
        text:"Application Menu is a menu element available to any view."
            +"<li>It can be opened by tapping on the menu title area."
            +"<li>It contains commands and view settings for the view in question and an optional area for style commands."
            +"<li>Each view of the application can have different menu contents."
            +"<b>Guidelines</b> for mandatory menu items:"
            +"<li>Settings should be on the main View of the application, unless the settings are specific to a sub-View"
            +"<li>Help should be available in each View"
            +"<li>Exit task should be present in all Views except for the main View of the application."
    }
}
