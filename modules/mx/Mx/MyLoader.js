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
    if (delegate.isReady) {
        dynamicObject = delegate.createObject();
        dynamicObject.parent = tooltipLoader;
    } else {
        print(delegate.errorsString());
    }
}
