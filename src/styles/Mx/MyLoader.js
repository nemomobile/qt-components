var dynamicObject = null;

function handleLoadedChange()
{
    if (loaded) {
        if (dynamicObject == null) {
            createDynamicObject();
        }
    } else if (dynamicObject != null) {
        dynamicObject.destroy();
        dynamicObject = null;
    }
}

function createDynamicObject()
{
    if (delegate.status == Component.Ready) {
        dynamicObject = delegate.createObject(tooltipLoader);
    } else {
        print(delegate.errorsString());
    }
}
