// Helper code for the button-group.qml example

var buttonList = [];
var count = 0;

function addButton(parent) {
    var button = createQmlObject("import Mx 1.0; Button { id: me; checkable: true; text: 'Button " + count + "' }", parent);
    count += 1;
    buttonList.push(button);
}

function removeLastButton() {
    if (buttonList.length > 0) {
        var last = buttonList.pop();
        last.destroy();
    }
}

function removeButton(button) {
    for (var i = 0; i < buttonList.length; i++) {
        if (buttonList[i] === button) {
            buttonList.splice(i, 1);
            break;
        }
    }

    button.destroy();
}


