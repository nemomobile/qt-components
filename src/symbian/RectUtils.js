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
.pragma library

function rectContainsPoint(rect, x, y) {
    return rect.x < x
           && x < rect.x + rect.width
           && rect.y < y
           && y < rect.y + rect.height;
}

function rectIntersectsRect(rect1, rect2) {
    return !(rect2.x > rect1.x + rect1.width  || rect2.x + rect2.width < rect1.x ||
             rect2.y > rect1.y + rect1.height || rect2.y + rect2.height < rect1.y);
}

// returns true if rect1 contains rect2
function rectContainsRect(rect1, rect2) {
    return (rect1.x < rect2.x && rect2.x + rect2.width < rect1.x + rect1.width &&
            rect1.y < rect2.y && rect2.y + rect2.height < rect1.y + rect1.height);
}

function manhattan(point1, point2) {
    return Math.abs(point1.x - point2.x) + Math.abs(point1.y - point2.y)
}
