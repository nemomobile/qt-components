import Qt 4.7

//Not a first release sort of component
Item{
    property model model//with options of 'text' and 'icon' roles, but a QStringList should work
    signal selected(string)
    signal selected(index)
    signal clicked
    signal onReturnPressed
    property Completer completer//QCompleter, possibly
    property Validator validator//defaults to a QValidator of whether it's a start of a string in the string list or not
    property string currentText: ''
}
