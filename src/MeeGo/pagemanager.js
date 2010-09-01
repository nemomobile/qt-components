// a list of all pages
// the last page is currently visible
var allPages = [];

function nextPage(newPageComponent, parent)
{
    var newPage = newPageComponent.createObject(parent)

    if (allPages.length > 0) {
        var oldPage = allPages[allPages.length-1];
        oldPage.__pageNavigationState = "left";
        newPage.__pageNavigationState = "right";

        titlebar.showBackButton = true
    }

    newPage.__animationEnabled = true;
    newPage.__pageNavigationState = "";

    allPages.push(newPage);
    return newPage;
}

function prevPage()
{
    if (allPages.length < 1) {
        console.log("WARNING: previous page called on first page");
        return null;
    }

    var currentPage = allPages.pop();
    currentPage.__pageNavigationState = "right";

    var newPage = allPages[allPages.length-1];
    newPage.__pageNavigationState = "";

    if (allPages.length == 1)
        titlebar.showBackButton = false;

    return newPage;
}
