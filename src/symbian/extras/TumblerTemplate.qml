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

import Qt 4.7
import com.nokia.symbian 1.0
import "." 1.0
import "Constants.js" as C

Item {
    id: template
    objectName: "tumblerColumn" + index

    property Item tumblerColumn
    property alias pathView: pView
    property int index: -1
    property bool firstColumn: false

    opacity: enabled ? C.TUMBLER_OPACITY_FULL : C.TUMBLER_OPACITY
    width: childrenRect.width
    visible: tumblerColumn ? tumblerColumn.visible : false
    enabled: tumblerColumn ? tumblerColumn.enabled : true

    Image {
        id: divider
        anchors.left: parent.left
        height: firstColumn ? 0 : pView.height
        width: firstColumn ? 0 : Math.round(platformStyle.graphicSizeTiny / 2)
        source: privateStyle.imagePath("qtg_fr_tumbler_divider")
    }

    PathView {
        id: pView

        anchors.left: divider.right
        model: tumblerColumn ? tumblerColumn.items : undefined
        currentIndex: tumblerColumn ? tumblerColumn.selectedIndex : 0
        // highlight locates in the middle (ratio 0.5) if items do not fully occupy the Tumbler
        preferredHighlightBegin: privateStyle.menuItemHeight * pView.count > height ?
                                     (pView.height / 2) / (privateStyle.menuItemHeight * pView.count) : 0.5
        preferredHighlightEnd: preferredHighlightBegin
        highlightRangeMode: PathView.StrictlyEnforceRange
        clip: true
        delegate: defaultDelegate
        highlight: defaultHighlight
        interactive: template.enabled
        width: tumblerColumn ? tumblerColumn.width - divider.width : 0
        height: root.height - container.height // decrease by text

        onMovementStarted: {
            internal.movementCount++;
        }
        onMovementEnded: {
            internal.movementCount--;
            root.changed(template.index) // got index from delegate
        }

        path: Path {
            startX: pView.width / 2;
            startY: privateStyle.menuItemHeight * pView.count > pView.height ?
                        0 : (pView.height - privateStyle.menuItemHeight * pView.count) / 2
             PathLine {
                 x: pView.width / 2
                 y: privateStyle.menuItemHeight * pView.count > pView.height ?
                        privateStyle.menuItemHeight * pView.count : (pView.height + privateStyle.menuItemHeight * pView.count) / 2
             }
        }
    }

    Item {
        id: container
        anchors.top: pView.bottom
        width: tumblerColumn ? tumblerColumn.width : 0
        height: internal.hasLabel ? privateStyle.menuItemHeight : 0 // internal.hasLabel is from root tumbler

        Text {
            id: label

            text: tumblerColumn ? tumblerColumn.label : ""
            elide: Text.ElideRight
            horizontalAlignment: "AlignHCenter"
            verticalAlignment: "AlignVCenter"
            color: platformStyle.colorNormalLight
            font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
            anchors.fill: parent
        }
    }

    Component {
        id: defaultDelegate

        Item {
            id: delegateItem
            width: tumblerColumn.width
            height: privateStyle.menuItemHeight

            Text {
                text: !!value ? value : ""
                elide: Text.ElideRight
                horizontalAlignment: "AlignHCenter"
                verticalAlignment: "AlignVCenter"
                color: delegateItem.PathView.isCurrentItem ? platformStyle.colorHighlighted : platformStyle.colorNormalLight
                font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeLarge }
                anchors.fill: parent

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (pView.interactive) {
                            tumblerColumn.selectedIndex = index;  // got index from delegate
                            root.changed(template.index);
                        }
                    }
                }
            }
        }
    }

    Component {
        id: defaultHighlight

        Image {
            id: highlight
            objectName: "highlight"
            width: tumblerColumn ? tumblerColumn.width : 0
            height: privateStyle.menuItemHeight
            source: privateStyle.imagePath("qtg_fr_tumbler_highlight")
            fillMode: Image.TileHorizontally
        }
    }
}
