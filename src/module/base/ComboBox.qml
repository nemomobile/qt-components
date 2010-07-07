import Qt 4.7

Item{
    property model model//with options of 'text' and 'icon' roles, but a QStringList should work
    property bool open
    signal selected(index)
}
