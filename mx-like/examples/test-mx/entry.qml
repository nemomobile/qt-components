import Qt 4.7
import Mx 1.0 as Mx

Item {
    Column {
        spacing: 10;

        Mx.Entry {
            width: 240;
            text: "Hello World!";
            hint: "Fixed Width Entry";
        }
        Mx.Entry {
            hint: "Search...";
            leftIconSource: "images/edit-find.png";
            onLeftIconClicked: text="Hello world";
            rightIconSource: "images/edit-clear.png";
            onRightIconClicked: text="";
        }

        Mx.Entry {
            id: textSetable;
            hint: "hint hint...";
        }
        Row {
            spacing: 10;
            Mx.Button {
                text: "Set";
                onClicked: textSetable.text = "Here is some text";
            }
            Mx.Button {
                text: "Clear";
                onClicked: textSetable.text = "";
            }
        }

        Mx.Entry {
            hint: "Secret!";
            secret: true;
        }
    }
}