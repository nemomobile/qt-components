import Qt 4.7

/*
    The ButtonGroup also has the mandate of grouping any GroupedButtons which are its children.
    This refers to both visual grouping, and setting them to be mutally exclusive iff exclusive is true
    Visual grouping in a style dependant way is the main point of the ButtonGroup.
*/
Item {
    property string groupName
    property bool exclusive: true
}
