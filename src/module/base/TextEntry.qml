import Qt 4.7

Item{//plain text edit, can be multi-line
    property string text: ''
    property int maxLines: 1//0 means unlimited
    property int maxLength: 0 //0 means unlimited

    property bool readOnly: false
    property TextInput.EchoMode echoMode

    property validator validator: null
    property Qt.InputMethodHints inputMethodHints
    signal onAccepted(text)//only emitted when validator says okay and they tap/return/whatever
}
