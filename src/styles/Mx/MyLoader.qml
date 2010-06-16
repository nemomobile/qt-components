import Qt 4.7
import "MyLoader.js" as Helper

// This components mimics the behavior of Loader but adds
// the property "loaded" we need.
// ### Hopefully we can use an autentic Loader in the future.

Item {
    id: tooltipLoader;
    property Component delegate;
    property bool loaded;

    onLoadedChanged: Helper.handleLoadedChange();
}
