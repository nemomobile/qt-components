import Qt 4.7
import com.nokia.symbian 1.0

Item {

    property string currentToolsName: toolbar.tools == undefined ? "undefined" : toolbar.tools.objectName

    ToolBar {
        id: toolbar
    }

    ToolBarLayout {
        id: commonToolsA
        visible: false
        objectName: "toolsA"
        ToolButton { iconSource: "qrc:tb_plus.svg"}
        ToolButton { text: "test"}
        ToolButton { iconSource: "qrc:tb_plus.svg"}
    }

    ToolBarLayout {
        id: commonToolsB
        visible: false
        objectName: "toolsB"
        ToolButton { iconSource: "qrc:tb_plus.svg" }
        ToolButton { iconSource: "qrc:tb_plus.svg" }
        ToolButton { iconSource: "qrc:tb_plus.svg" }
    }

    function setToolsA()
    {
        toolbar.setTools(commonToolsA)
    }

    function setToolsB()
    {
        toolbar.setTools(commonToolsB)
    }

    function setToolsToNull() {
        try {
            toolbar.setTools(null);
        } catch (e) {
            return false;
        }
        return true;
    }

    function getLayout(which){
        if (which == "commonToolsA")
            return commonToolsA
        else if (which == "commonToolsB")
            return commonToolsB
        return ""
    }
}
