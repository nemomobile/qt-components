import Qt 4.7
import com.meego 1.0

Item {
    id: container
    property int dynPageCount: 0

    property variant pages: [pageStatic, pageStatic2, pageStatic3]

    property alias depth: pageStack.depth

    property string currentPageName: pageStack.currentPage==undefined? "0" : pageStack.currentPage.objectName

    PageStack {
        id: pageStack
    }

    Component {
        id: pageComponent
        Page {
            id: page
            objectName: "dynamicpage"
        }
    }

    Page {
        id: pageStatic
        objectName: "page1"
    }

    Page {
        id: pageStatic2
        objectName: "page2"
    }

    Page {
        id: pageStatic3
        objectName: "page3"
    }


    function pushPage()
    {
        pageStack.push(pageComponent);
    }

    function replacePage()
    {
        pageStack.replace(pageStatic3);
    }

    function pushPageStatic()
    {
        pageStack.push(pageStatic);
    }

    function popPage()
    {
        return pageStack.pop();
    }

    function clearStack()
    {
        return pageStack.clear();
    }

    function pushPageArray()
    {
        return pageStack.push(pages);
    }


}
