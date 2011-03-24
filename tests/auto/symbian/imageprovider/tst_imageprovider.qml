import Qt 4.7
import com.nokia.symbian 1.0

Window {
    id: window
    Image {
        id: image
        objectName:  "image"
        source: "image://theme/list1.png"
        sourceSize.width: 60
        sourceSize.height: 60
        width: 100
        height: 100
        property bool loadOk: status != Image.Error
        fillMode: Image.PreserveAspectFit
    }

    BorderImage {
        id: borderImage
        objectName: "borderImage"
        anchors.top: image.bottom
        width: 200
        height: 100
        property bool loadOk: status != Image.Error
        border { left: 20; top: 20; right: 20; bottom: 20 }
    }
}
