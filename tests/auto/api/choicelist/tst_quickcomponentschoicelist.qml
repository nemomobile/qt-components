import Qt 4.7
import com.meego 1.0

ChoiceList {
    id: testTarget
    width: 640; height: 480

    model: 5
    delegate: Rectangle {
        Text {
            text: "I'm item number'" + index
        }
    }
}
