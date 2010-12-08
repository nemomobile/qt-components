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
import com.meego.themebridge 1.0

ImplicitSizeItem {
    id: root

    property alias checked: thumb.checked

    implicitWidth: meegostyle.preferredWidth
    implicitHeight: meegostyle.preferredHeight

    Style {
        id: meegostyle
        styleClass: "MButtonSwitchStyle"
        mode: state=="on" ? "selected" : "normal"
    }

    Background {
        anchors.fill: parent
        style: meegostyle
    }

    // XXX This is not exactly equal to how MButtonSwitchView composes the image
    //     but gets close.
    //     Proper implementation probably requires merging of both standard and
    //     selected image
    MaskedImage {
        id: selectedImage
        anchors.fill: root
        style: meegostyle
        maskProperty: "sliderMask"
        imageProperty: "sliderImageSelected"
        opacity: (thumb.x - mousearea.drag.minimumX) / mousearea.drag.maximumX
    }

    MaskedImage {
        anchors.fill: root
        style: meegostyle
        maskProperty: "sliderMask"
        imageProperty: "sliderImage"
        opacity: 1 - selectedImage.opacity
    }

    ScalableImage {
        id: thumb
        anchors.verticalCenter: parent.verticalCenter
        property bool checked: false

        height: root.height - 2 * meegostyle.current.get("thumbMargin")
        width: height // XXX Assuming 1:1 aspect ratio. This is not safe

        style: meegostyle
        imageProperty: "thumbImage"

        states: [
            State {
                name: "off"
                when: !mousearea.drag.active && !checked
                PropertyChanges {
                    restoreEntryValues: false
                    target: thumb
                    x: mousearea.drag.minimumX
                }
            },
            State {
                name: "on"
                when: !mousearea.drag.active && checked
                PropertyChanges {
                    restoreEntryValues: false
                    target: thumb
                    x: mousearea.drag.maximumX
                }
            },
            State {
                name: "dragging"
                when: mousearea.drag.active
            }]

        transitions:
            Transition {
                SmoothedAnimation { properties: "x"; velocity: 500; maximumEasingTime: 0 }
            }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        anchors.leftMargin: -meegostyle.current.get("reactiveMarginLeft")
        anchors.rightMargin: -meegostyle.current.get("reactiveMarginRight")
        anchors.topMargin: -meegostyle.current.get("reactiveMarginTop")
        anchors.bottomMargin: -meegostyle.current.get("reactiveMarginBottom")

        drag {
            axis: Drag.XAxis
            minimumX: meegostyle.get("thumbMargin")
            maximumX: parent.width - thumb.width - meegostyle.get("thumbMargin")

            target: thumb
            onActiveChanged: {
                if (!mousearea.drag.active) {
                    thumb.checked = (thumb.x > mousearea.drag.maximumX / 2)
                }
            }
        }

        onClicked: thumb.checked = !thumb.checked
    }
}
