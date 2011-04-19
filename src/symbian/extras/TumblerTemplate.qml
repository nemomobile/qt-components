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
    property bool lastColumn: false

    opacity: enabled ? C.TUMBLER_OPACITY_FULL : C.TUMBLER_OPACITY
    width: childrenRect.width
    visible: tumblerColumn ? tumblerColumn.visible : false
    enabled: tumblerColumn ? tumblerColumn.enabled : true
    height: parent ? parent.height : undefined

    Image {
        id: divider
        anchors.left: parent.left
        height: pView.height
        source: privateStyle.imagePath("qtg_graf_tumbler_divider")
    }

    PathView {
        id: pView

        anchors.left: divider.right
        model: tumblerColumn ? tumblerColumn.items : undefined
        currentIndex: tumblerColumn ? tumblerColumn.selectedIndex : 0
        preferredHighlightBegin: (height / 2) / (C.TUMBLER_ROW_HEIGHT * pView.count)
        preferredHighlightEnd: preferredHighlightBegin
        highlightRangeMode: PathView.StrictlyEnforceRange
        clip: true
        delegate: defaultDelegate
        highlight: defaultHighlight
        interactive: template.enabled
        width: tumblerColumn ? tumblerColumn.width - C.TUMBLER_BORDER_MARGIN : 0
        height: root.height - container.height - 2*C.TUMBLER_BORDER_MARGIN // decrease by text & border heights

        onMovementStarted: {
            internal.movementCount++;
        }
        onMovementEnded: {
            internal.movementCount--;
            root.changed(template.index) // got index from delegate
        }

        path: Path {
             startX: template.width / 2; startY: 0
             PathLine {
                 x: template.width / 2
                 y: C.TUMBLER_ROW_HEIGHT * pView.count
             }
        }
    }

    Image {
        id: divider2
        anchors.right: pView.right
        height: lastColumn ? pView.height : 0
        source: privateStyle.imagePath("qtg_graf_tumbler_divider")
    }

    Item {
        id: container
        anchors.top: pView.bottom
        width: tumblerColumn ? tumblerColumn.width : 0
        height: internal.hasLabel ? C.TUMBLER_LABEL_HEIGHT : 0 // internal.hasLabel is from root tumbler

        Text {
            id: label

            text: tumblerColumn ? tumblerColumn.label : ""
            elide: Text.ElideRight
            horizontalAlignment: "AlignHCenter"
            color: C.TUMBLER_COLOR_LABEL
            font { family: C.FONT_FAMILY_LIGHT; pixelSize: C.FONT_LIGHT_SIZE }
            anchors { fill: parent; margins: C.TUMBLER_MARGIN}
        }
    }

    Component {
        id: defaultDelegate

        Item {
            width: tumblerColumn.width
            height: C.TUMBLER_ROW_HEIGHT

            Text {
                text: !!value ? value : ""
                elide: Text.ElideRight
                horizontalAlignment: "AlignHCenter"
                color: C.TUMBLER_COLOR_TEXT
                font { family: C.FONT_FAMILY_BOLD; pixelSize: C.FONT_DEFAULT_SIZE }
                anchors { fill: parent; margins: C.TUMBLER_MARGIN }

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
            source: privateStyle.imagePath("qtg_graf_tumbler_highlighted")
            fillMode: Image.TileHorizontally
        }
    }
}
