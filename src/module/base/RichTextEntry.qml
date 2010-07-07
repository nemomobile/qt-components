import Qt 4.7

//Not a component for the first pass
Item{
    property string text: ''
    property bool readOnly: false
    property int maxLines: 0

    property Qt.InputMethodHints inputMethodHints
}
