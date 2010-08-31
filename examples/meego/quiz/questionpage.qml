
import Qt 4.7
import com.meego 1.0


Page {
    id: questionPage;
    property Component answer

    Label {
        id: question;
        text: "Would you like some tea?";
        width: parent.width;
        anchors.top: parent.top;
        anchors.bottom: yes.top;
        anchors.centerIn: parent;
    }

    Button  {
        id: yes;

        anchors.left: parent.left;
        anchors.bottom: parent.bottom;

        text: "Yes I love tea";
        onClicked: {
            window.nextPage(window.answerPage);
            window.currentPage.text = "Then go make it!";
        }
    }

    Button  {
        id: no;
        anchors.left: yes.right;
        anchors.bottom: parent.bottom;
        anchors.right: parent.right;
        text: "No";
        onClicked: {

            window.nextPage(window.answerPage);
            window.currentPage.text = "But it is such a nice lovely great tea that I was going to prepare for you...";
        }
    }
}
