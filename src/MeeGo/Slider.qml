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
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

Item {
    id: slider

    property bool vertical: false // XXX to be implemented

    Component.onCompleted: {
        // XXX This is not nice
        // What I wanted to do was to bind the root item "implicitSize" to the
        // size provided by meegostyle. However:
        //  - Implicit size is not exported to QML, that's C++ accessible only
        //  - Binding actual width and height to the sizes provided by meegostyle
        //    is not OK. It can cause sizes set externally to be overriden if
        //    the binding is re-evaluated.
        width = meegostyle.preferredWidth
        height = meegostyle.preferredHeight
    }

    Style {
        id: meegostyle
        styleType: Style.Slider
        mode: "pressed"
    }

    Background {
        id: background
        anchors.fill: parent
        style: meegostyle
    }

    ScalableImage {
        id: groove

        anchors.left: parent.left
        anchors.leftMargin: 20 // XXX get from meegostyle
        anchors.right: parent.right
        anchors.rightMargin: 20 // XXX get from meegostyle
        anchors.verticalCenter: parent.verticalCenter

        height: 10 // XXX get from meegostyle

        style: meegostyle
        imageProperty: "backgroundBaseImage"

        MouseArea {
            id: grooveArea
            anchors.fill: parent

            onPressed: elapsedModel.setPosition(mouse.x - handle.width / 2)
        }

        ScalableImage {
            id: received

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: receivedModel.position

            style: meegostyle
            imageProperty: "backgroundReceivedImage"
        }

        ScalableImage {
            id: elapsed

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: handle.horizontalCenter

            style: meegostyle
            imageProperty: "backgroundElapsedImage"
        }

        // XXX This needs to show pixmaps from MT. Probably
	//     it's better to make our own Pixmap primitive
        Rectangle {
            id: handle

            width: 30
            height: width
            radius: width/2

            color: "darkgray"

            anchors.verticalCenter: parent.verticalCenter
            x: elapsedModel.position

            MouseArea {
                anchors.fill: parent

                drag.axis: Drag.XAxis
                drag.target: parent
                drag.minimumX: elapsedModel.positionAtMinimum
                drag.maximumX: elapsedModel.positionAtMaximum
            }
        }
    }

    RangeModel {
        id: elapsedModel
        position: handle.x
        positionAtMinimum: -handle.width / 2
        positionAtMaximum: groove.width - handle.width / 2
    }

    RangeModel {
        id: receivedModel
        positionAtMinimum: elapsedModel.positionAtMinimum
        positionAtMaximum: elapsedModel.positionAtMaximum
        minimumValue: elapsedModel.minimumValue
        maximumValue: elapsedModel.maximumValue
        value: 80
    }
}
