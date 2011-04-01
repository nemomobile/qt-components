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
import "." 1.0
import "AppManager.js" as Utils

ImplicitSizeItem {
    id: root

    // Common Public API
    property Item tab
    property bool checked: internal.tabGroup != null && internal.tabGroup.currentTab == tab
    property bool pressed: stateGroup.state == "Pressed" && mouseArea.containsMouse
    property alias text: label.text
    property alias iconSource: imageLoader.source

    signal clicked

    implicitWidth: Math.max(2 * imageLoader.width, 2 * platformStyle.paddingMedium + privateStyle.textWidth(label.text, label.font))
    implicitHeight: internal.portrait ? privateStyle.tabBarHeightPortrait : privateStyle.tabBarHeightLandscape

    QtObject {
        id: internal

        property Item tabGroup: Utils.findParent(tab, "currentTab")
        property bool portrait: screen.currentOrientation == Screen.Portrait || screen.currentOrientation == Screen.PortraitInverted

        function press() {
            privateStyle.play(Symbian.BasicButton)
        }
        function click() {
            root.clicked()
            privateStyle.play(Symbian.BasicButton)
            if (internal.tabGroup)
                internal.tabGroup.currentTab = tab
        }
    }

    StateGroup {
        id: stateGroup

        states: [
            State { name: "Pressed" },
            State { name: "Canceled" }
        ]
        transitions: [
            Transition {
                to: "Pressed"
                ScriptAction { script: internal.press() }
            },
            Transition {
                from: "Pressed"
                to: ""
                ScriptAction { script: internal.click() }
            }
        ]
    }

    BorderImage {
        id: background

        function postfix() {
            if (root.pressed)
                return "passive_pressed"
            else if (root.checked)
                return "active"
            else
                return "passive_normal"
        }

        source: privateStyle.imagePath("qtg_fr_tab_" + postfix())
        anchors.fill: parent
        border {
            left: platformStyle.borderSizeMedium
            top: platformStyle.borderSizeMedium
            right: platformStyle.borderSizeMedium
            bottom: platformStyle.borderSizeMedium
        }
    }

    Text {
        id: label

        objectName: "label"
        // hide in landscape if icon is present
        visible: !(iconSource.toString() && !internal.portrait)
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: platformStyle.paddingMedium
            rightMargin: platformStyle.paddingMedium
            bottomMargin: iconSource.toString()
                ? platformStyle.paddingSmall
                : (parent.height - label.height) / 2
        }
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignHCenter
        font { family: platformStyle.fontFamilyRegular; pixelSize: platformStyle.fontSizeSmall }
        color: {
            if (root.pressed)
                platformStyle.colorPressed
            else if (root.checked)
                platformStyle.colorNormalLight
            else
                platformStyle.colorNormalMid
        }
    }

    Loader {
        // imageLoader acts as wrapper for Image and Icon items. The Image item is
        // shown when the source points to a image (jpg, png). Icon item is used for
        // locigal theme icons which are colorised.
        id: imageLoader

        property url source
        property string iconId

        // function tries to figure out if the source is image or icon
        function inspectSource() {
            var fileName = imageLoader.source.toString()
            if (fileName.length) {
                fileName = fileName.substr(fileName.lastIndexOf("/") + 1)
                var fileBaseName = fileName
                if (fileName.indexOf(".") >= 0)
                    fileBaseName = fileName.substr(0, fileName.indexOf("."))

                // empty file extension and .svg are treated as icons
                if (fileName == fileBaseName || fileName.substr(fileName.length - 4).toLowerCase() == ".svg")
                    imageLoader.iconId = fileBaseName
                else
                    imageLoader.iconId = ""
            } else {
                imageLoader.iconId = ""
            }
        }

        Component.onCompleted: inspectSource()
        onSourceChanged: inspectSource()

        width : platformStyle.graphicSizeSmall
        height : platformStyle.graphicSizeSmall
        sourceComponent: {
            if (iconId)
                return iconComponent
            if (source.toString())
                return imageComponent
            return undefined
        }
        anchors {
            top: parent.top
            topMargin : !parent.text || !internal.portrait
                ? (parent.height - imageLoader.height) / 2
                : platformStyle.paddingSmall
            horizontalCenter: parent.horizontalCenter
        }

        Component {
            id: imageComponent

            Image {
                id: image

                objectName: "image"
                source: imageLoader.iconId ? "" : imageLoader.source
                sourceSize.width: width
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                smooth: true
                anchors.fill: parent
            }
        }

        Component {
            id: iconComponent

            Icon {
                id: icon

                objectName: "icon"
                anchors.fill: parent
                iconColor: label.color
                iconName: imageLoader.iconId
            }
        }
    }

    MouseArea {
        id: mouseArea

        onPressed: stateGroup.state = "Pressed"
        onReleased: stateGroup.state = ""
        onCanceled: {
            // Mark as canceled
            stateGroup.state = "Canceled"
            // Reset state
            stateGroup.state = ""
        }
        onExited: stateGroup.state = "Canceled"

        anchors.fill: parent
    }
}
