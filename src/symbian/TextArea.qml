/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
import "themebridge" 1.0

ImplicitSizeItem {
    id: root

    // Common public API
    property alias font: textEdit.font
    property alias cursorPosition: textEdit.cursorPosition
    property alias horizontalAlignment: textEdit.horizontalAlignment
    property alias verticalAlignment: textEdit.verticalAlignment
    property alias readOnly: textEdit.readOnly
    property alias selectedText: textEdit.selectedText
    property alias selectionEnd: textEdit.selectionEnd
    property alias selectionStart: textEdit.selectionStart
    property alias text: textEdit.text
    property alias textFormat: textEdit.textFormat
    property alias wrapMode: textEdit.wrapMode

    function copy() {
        textEdit.copy()
    }

    function paste() {
        textEdit.paste()
    }

    function cut() {
        textEdit.cut()
    }

    function select(start, end) {
        textEdit.select(start, end)
    }

    function selectAll() {
        textEdit.selectAll()
    }

    function selectWord() {
        textEdit.selectWord()
    }

    function positionAt(x, y) {
        // ### TODO: remove the left margins from x
        return textEdit.positionAt(x, y)
    }

    function positionToRectangle(pos) {
        // ### TODO: translate rect to TextArea coord
        return textEdit.positionToRectangle(pos)
    }

    // API extensions
    property alias placeholderText: placeholder.text
    // TODO: Refactor implicit size when following bugs are resolved
    // http://bugreports.qt.nokia.com/browse/QTBUG-14957
    // http://bugreports.qt.nokia.com/browse/QTBUG-16665
    // http://bugreports.qt.nokia.com/browse/QTBUG-16710 (fixed in Qt 4.7.2)
    // http://bugreports.qt.nokia.com/browse/QTBUG-12305 (fixed in QtQuick1.1)
    property real maxImplicitWidth: parent ? parent.width : screen.width
    property real maxImplicitHeight: parent ? parent.height : screen.height

    implicitWidth: {
        var preferredWidth = Math.max(flick.contentWidth, privy.minImplicitWidth)
        preferredWidth += container.anchors.leftMargin + container.anchors.rightMargin
        return Math.min(preferredWidth, root.maxImplicitWidth)
    }

    implicitHeight: {
        var preferredHeight = Math.max(flick.contentHeight, privy.minImplicitHeight)
        preferredHeight += container.anchors.topMargin + container.anchors.bottomMargin
        return Math.min(preferredHeight, root.maxImplicitHeight)
    }

    QtObject {
        id: privy
        // TODO: More consistent minimum width for empty TextArea than 20 * " " on current font?
        property real minImplicitWidth: placeholder.text ? placeholder.model.paintedWidth
                                                         : style.textWidth("                    ", style.current.get("font"))
        property real minImplicitHeight: placeholder.text ? placeholder.model.paintedHeight
                                                          : style.fontHeight(style.current.get("font"))
    }

    Style {
        id: style
        mode: textEdit.activeFocus ? "selected" : "default"
        styleClass: "TextArea"
    }

    Frame {
        id: background
        anchors.fill: parent
        frameName: style.current.get("frameName")
        frameType: style.current.get("frameType")
    }

    Item {
        id: container
        anchors {
            leftMargin: style.current.get("leftMargin"); rightMargin: style.current.get("rightMargin")
            topMargin: style.current.get("topMargin"); bottomMargin: style.current.get("bottomMargin")
        }
        x: anchors.leftMargin
        y: anchors.topMargin
        height: parent.height - anchors.bottomMargin - y
        width: parent.width - anchors.rightMargin - x

        // TODO: Should placeholder also be scrollable?
        Text {
            id: placeholder
            objectName: "placeholder"

            // TODO: See TODO: Refactor implicit size...
            property variant model: Text {
                font: style.current.get("font")
                text: placeholder.text
                visible: false
                wrapMode: textEdit.wrapMode
                horizontalAlignment: textEdit.horizontalAlignment
                verticalAlignment: textEdit.verticalAlignment
                height: root.maxImplicitHeight - container.anchors.topMargin - container.anchors.bottomMargin
                width: root.maxImplicitWidth - container.anchors.leftMargin - container.anchors.rightMargin
                opacity: 0
            }

            color: style.current.get("placeholderColor")
            font: style.current.get("font")
            opacity: visible ? 1 : 0
            visible: (!textEdit.activeFocus || textEdit.activeFocus && textEdit.readOnly) && !textEdit.text && text
            wrapMode: textEdit.wrapMode
            height: parent.height
            width: parent.width
        }

        Flickable {
            id: flick

            function ensureVisible(rect) {
                if (Math.round(contentX) > Math.round(rect.x))
                    contentX = rect.x
                else if (Math.round(contentX + width) < Math.round(rect.x + rect.width))
                    contentX = rect.x + rect.width - width

                if (Math.round(contentY) > Math.round(rect.y))
                    contentY = rect.y
                else if (Math.round(contentY + height) < Math.round(rect.y + rect.height))
                     contentY = rect.y + rect.height - height
            }

            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            contentHeight: textEdit.model.paintedHeight
            contentWidth: textEdit.model.paintedWidth

            // To avoid too early loading of ScrollDecorator when Flickable is not ready
            Component.onCompleted: { scroll.flickableItem = flick }

            TextEdit {
                id: textEdit
                objectName: "textEdit"

                // TODO: See TODO: Refactor implicit size...
                property variant model: TextEdit {
                    font: style.current.get("font")
                    text: textEdit.text
                    horizontalAlignment: textEdit.horizontalAlignment
                    verticalAlignment: textEdit.verticalAlignment
                    wrapMode: textEdit.wrapMode
                    visible: false
                    opacity: 0
                    height: root.maxImplicitHeight - container.anchors.topMargin - container.anchors.bottomMargin
                    width: root.maxImplicitWidth - container.anchors.leftMargin - container.anchors.rightMargin
                }

                font: style.current.get("font")
                color: style.current.get("color")
                selectedTextColor: style.current.get("selectionTextColor")
                selectionColor: style.current.get("selectionColor")
                textFormat: TextEdit.AutoText
                wrapMode: TextEdit.Wrap
                // TODO: Use desktop text selection behaviour for now.
                selectByMouse: true
                height: container.height
                width: container.width
                // TODO: Make bug report?
                // Called too early (before TextEdit size is adjusted) delay ensureVisible call a bit
                onCursorRectangleChanged: delayedEnsureVisible.start()
                onFocusChanged: {
                    if (focus) {
                        scroll.flashDecorators()
                        // TODO: Enable when decision made about Qt Mobility usage
                        //style.play(ThemeEffect.Editor)
                    }
                }
            }
        }

        ScrollDecorator {
            id: scroll
        }

        Timer {
            id: delayedEnsureVisible
            interval: 1
            onTriggered: flick.ensureVisible(textEdit.cursorRectangle)
        }
    }
}
