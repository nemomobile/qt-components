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
import "Constants.js" as C
import "TumblerIndexHelper.js" as IH

Dialog {
    id: root

    property alias titleText: title.text
    property alias columns: tumbler.columns
    property alias acceptButtonText: acceptButton.text
    property alias rejectButtonText: rejectButton.text

    title: Text {
        id: title
        visible: text.length > 0
        anchors {
            left: parent.left; leftMargin: platformStyle.paddingLarge; right: parent.right;
            rightMargin: platformStyle.paddingLarge; verticalCenter: parent.verticalCenter
        }
        font { family: platformStyle.fontFamilyRegular; pixelSize: C.FONT_DEFAULT_SIZE }
        color: platformStyle.colorNormalLight
        elide: Text.ElideRight
    }

    content: Item {
        id: content
        height: tumbler.height + platformStyle.paddingLarge * 2
        width: parent.width

        Tumbler {
            id: tumbler
            anchors.centerIn: parent
            height: privateStyle.menuItemHeight * 4
            width: parent.width - platformStyle.paddingMedium * 4
            privateDelayInit: true
            states: State {
                when: screen.currentOrientation == Screen.Landscape || screen.currentOrientation == Screen.LandscapeInverted
                PropertyChanges {
                    target: tumbler; height: privateStyle.menuItemHeight * 3
                }
            }
        }
    }

    buttons: ToolBar {
        id: buttons
        width: parent.width

        Row {
            id: buttonRow
            anchors.centerIn: parent

            ToolButton {
                id: acceptButton
                width: buttons.width / 2
                onClicked: accept()
                visible: text != ""
            }
            ToolButton {
                id: rejectButton
                width: buttons.width / 2
                onClicked: reject()
                visible: text != ""
            }
        }
    }

    QtObject {
        id: internal
        property bool init: true
    }

    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            tumbler.privateInitialize();

            if (internal.init) {
                IH.saveIndex(tumbler);
                internal.init = false;
            }
            else {
                // restore index when dialog was canceled.
                // another case is when dialog was closed while tumbler was
                // still rotating (Qt sets the index to the last rotated
                // number, need to retore to a previously saved index in
                // this case)
                IH.restoreIndex(tumbler);
            }
        }
    }

    onAccepted: {
        tumbler.privateForceUpdate();
        IH.saveIndex(tumbler);
    }
}
