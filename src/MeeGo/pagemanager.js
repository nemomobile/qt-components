// a list of all pages containers, which contain the pages
// the last page container contains the currently visible page
var allPages = [];

function nextPage(newPageComponent, newPageContainer)
{
    var newPage = newPageComponent.createObject(newPageContainer)
    if (newPage == null)
        return null

    newPageContainer.page = newPage

    if (allPages.length > 0) {
        var oldPageContainer = allPages[allPages.length-1];
        oldPageContainer.state = "left";
        newPageContainer.state = "right";

        decoration.showBackButton = true
    }

    newPageContainer.__animationEnabled = true;
    newPageContainer.state = "";

    allPages.push(newPageContainer);
    return newPage;
}

function prevPage()
{
    if (allPages.length < 1) {
        console.log("WARNING: previous page called on first page");
        return null;
    }

    var currentPageContainer = allPages.pop();
    currentPageContainer.state = "right";

    var newPageContainer = allPages[allPages.length-1];
    newPageContainer.state = "";

    if (allPages.length == 1)
        decoration.showBackButton = false;

    return newPageContainer.page;
}
