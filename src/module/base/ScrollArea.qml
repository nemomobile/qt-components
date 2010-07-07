import Qt 4.7

Item{//On some platforms, scrolling is accomplished by flicking
    property bool interactive: true

    //TODO: replace with higher level API
    property int contentWidth: childrenRect.width
    property int contentHeight: childrenRect.height
    property int viewportX
    property int viewportY
    property int viewportHeight
    property int viewportWidth

    //Document as read only, will be in many implementations
    property bool scrolling
    property bool scrollingHorizontally
    property bool scrollingVertically

    //Convenience, with working defaults
    property bool atXEnd: viewportX + viewportWidth == contentWidth
    property bool atYEnd: viewportX + viewportHeight == contentHeight
    property bool atXStart: viewportX == 0
    property bool atYStart: viewportY == 0
}
