/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

import QtQuick 1.0

XmlListModel {
    property string tags : ""

    function commasep(x) {
        // Trim the list of tags
        var tmp = x.replace(/^\s+|\s+$/g,'');
        // Put a comma between each tag, which are separated by one or more spaces
        tmp = tmp.replace(/\s+/g,',');
        return tmp;
    }

    source: "http://api.flickr.com/services/feeds/photos_public.gne?"
            + (tags ? "tags="+commasep(tags)+"&" : "") + "format=rss2"
    query: "/rss/channel/item"
    namespaceDeclarations: "declare namespace media=\"http://search.yahoo.com/mrss/\";"

    XmlRole { name: "photoTitle"; query: "title/string()" }
    XmlRole { name: "photoThumbnailUrl"; query: "media:thumbnail/@url/string()" }
    XmlRole { name: "photoUrl"; query: "media:content/@url/string()" }
    XmlRole { name: "photoWidth"; query: "media:content/@width/number()" }
    XmlRole { name: "photoHeight"; query: "media:content/@height/number()" }
    XmlRole { name: "photoDescription"; query: "description/string()" }
    XmlRole { name: "photoTags"; query: "media:category/string()" }
    XmlRole { name: "photoAuthor"; query: "author/string()" }
    XmlRole { name: "photoDate"; query: "pubDate/string()" }
}
