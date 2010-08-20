import Qt 4.7
import com.meego 1.0


Page {


    id: answerPage;
    property string text: "empty"

    Label {
        id: label
        text: answerPage.text
        anchors.centerIn: parent
    }
}
