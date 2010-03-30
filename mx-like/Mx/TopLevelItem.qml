import Qt 4.7

Item {
    id: placeholder;
    default property alias data: topLevelItem.data;
    property alias transformOrigin: topLevelItem.transformOrigin;
    property alias scale: topLevelItem.scale;
    property alias rotation: topLevelItem.rotation;

    Item {
        id: topLevelItem;
        parent: topLevelParent(placeholder);
        x: mappedX(placeholder);
        y: mappedY(placeholder);
        width: placeholder.width;
        height: placeholder.height;

        opacity: placeholder.opacity;
        visible: placeholder.visible;
    }

    function topLevelParent(item)
    {
        while (item.parent.toString() != "null")
            item = item.parent;
        return item;
    }

    function mappedX(item)
    {
        var x = item.x;
        while (item.parent.toString() != "null") {
            item = item.parent;
            x += item.x;
        }
        return x;
    }
    function mappedY(item)
    {
        var y = item.y;
        while (item.parent.toString() != "null") {
            item = item.parent;
            y += item.y;
        }
        return y;
    }
}