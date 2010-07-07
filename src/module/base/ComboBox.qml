import Qt 4.7

Item{
    property model model//with options of 'text' and 'icon' roles, but a QStringList should work
    signal selected(string)
    signal selected(index)
    signal clicked
}
