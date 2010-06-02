import Qt 4.7
import Mx 1.0

Button {
    id: button
    property alias operation: button.text
    onClicked: doOp(text);

    width: 50; height: 30;
}
