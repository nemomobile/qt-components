import Qt 4.7

Item{//plain text edit, can be multi-line - rich text edit may or may not come later.
    property string text: ''
    property int maxLines: 1//0 means unlimited
    property int maxLength: 0 //0 means unlimited

    property bool readOnly: false
    property TextInput.EchoMode echoMode

    property validator validator: null
    property bool acceptableInput: true
    property Qt.InputMethodHints inputMethodHints
}
