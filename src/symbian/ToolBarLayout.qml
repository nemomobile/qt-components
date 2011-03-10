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

// ToolBarLayout is a container for items on a toolbar that automatically
// implements an appropriate layout for its children.

import Qt 4.7
import "." 1.0

ImplicitSizeItem {
    id: toolBarLayout

    //Determines wether the first child (if it has iconSource) is used as backButton
    //Backbutton occupies the leftmost space in layout. If there is no backbutton
    //this space is always empty
    property bool backButton: true

    implicitWidth: style.current.preferredWidth
    implicitHeight: style.current.preferredHeight
    visible: false

    Style {
        id: style
        styleClass: "ToolBar"
    }

    Style {
        id: buttonStyle
        styleClass: "ToolButton"
        onCurrentStyleChanged:toolBarLayout.layoutChildren()
    }

    function buttonWidth(child) {
        if (child.hasOwnProperty("implicitWidth")) {
            //ImplicitWidth for the ToolButton returns wrong value right after
            //orientation change so to make sure everything is ok ask it
            //directly from style. Buttons would work fine so to distinguish them from
            //ToolButtons check that property flat exists in child.
            if (child.hasOwnProperty("iconSource") && child.iconSource == "" && child.flat != undefined)
                return buttonStyle.current.get("textButtonWidth")
            return child.implicitWidth
        }
        return child.width
    }

    function layoutChildren() {
        //Layouting rules for symbian toolbar
        //1. BackButton occupies the leftmost space separate with margin in landscape
        //if there is no backbutton the space is left empty
        //
        //2.last icon button occupies rightmost space. If there is no rightmost iconbutton
        //the space can be use for something else
        //
        //3. If the remaining items are text buttons, they are distributed around
        //the center of empty space in the middle of toolbar
        //
        //4. If the remaining items are iconbuttons the they are distributed evenly along
        //the toolbar area

        if (parent == null || toolBarLayout.width == 0)
            return

        var numChildren = children.length
        for (var i = 0; i < children.length; ++i) {
            //make sure all the children have correct parent and y
            children[i].parent = toolBarLayout
            children[i].y = toolBarLayout.y
        }
        var horizontalMargin = style.current.get("border-margin") * 2
        var leftMargin = horizontalMargin + buttonStyle.current.get("iconButtonWidth")
        var rightMargin = toolBarLayout.width  - horizontalMargin

        // |   X   |
        if (numChildren == 1) {
            if (children[0].iconSource != ""){
                if (backButton)
                    children[0].x = horizontalMargin
                else
                    children[0].x = toolBarLayout.width - buttonWidth(children[0]) - horizontalMargin
            }
            else
                children[0].x = width / 2 - buttonWidth(children[0]) / 2
            return
        }

        var leftItem = (children[0].hasOwnProperty("iconSource") && backButton && children[0].iconSource != "") ? children[0] : undefined
        var rightItem = (children[numChildren-1].hasOwnProperty("iconSource") && children[numChildren-1].iconSource != "" ) ? children[numChildren-1] : undefined

        //for |X ... X| ToolButtons with icons
        if (leftItem != undefined){
            leftItem.x = toolBarLayout.x + horizontalMargin
            leftItem.y = toolBarLayout.y
            leftMargin = leftItem.x + buttonWidth(leftItem)
        }

        if (rightItem != undefined){
            rightItem.x = toolBarLayout.width - buttonWidth(rightItem) - horizontalMargin
            rightItem.y = toolBarLayout.y
            rightMargin = rightItem.x
        }

        var childsLeft = numChildren - (leftItem ? 1 : 0) - (rightItem ? 1 : 0)

        if (!childsLeft)
            return;

        //Lone child always goes to middle
        if (childsLeft == 1) {
            children[leftItem ? 1 : 0].x = toolBarLayout.width / 2 - buttonWidth(children[leftItem ? 1 : 0]) / 2
        }
        else {
            //text buttons are distributed around the center
            if (childsLeft == 2 && children[leftItem ? 1 : 0].hasOwnProperty("iconSource") && children[leftItem ? 1 : 0].iconSource == "") {
                children[leftItem ? 1 : 0].x = toolBarLayout.width / 2 - buttonWidth(children[leftItem ? 1 : 0])
                children[leftItem ? 2 : 1].x = toolBarLayout.width / 2
            }
            else{
                //iconic buttons are deployed evenly
                var spaceLeft = rightMargin - leftMargin
                var spacing = (spaceLeft - buttonWidth(children[1]) * childsLeft) / (childsLeft + 1)
                var curPos = leftMargin + spacing
                for (var p = leftItem ? 1 : 0; p < (rightItem ? (numChildren-1) : numChildren); p++) {
                    children[p].x = curPos
                    curPos += spacing + buttonWidth(children[p])
                }
            }
        }
    }

    Component.onCompleted: layoutChildren()
    onChildrenChanged: layoutChildren()  
    onBackButtonChanged: layoutChildren()
    onImplicitWidthChanged: layoutChildren()
}
