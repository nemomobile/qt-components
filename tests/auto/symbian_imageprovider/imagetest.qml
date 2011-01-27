import Qt 4.7
import com.nokia.symbian 1.0
import com.nokia.symbian.themebridge 1.0
Window {
    id: win
    Image {
        id: image
        source: "image://theme/list1.png";
        sourceSize.width:60
        sourceSize.height:60
        width: 100
        height:100
        fillMode: Image.PreserveAspectFit
    }
}
