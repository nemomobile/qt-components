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

/////////////////////////////////////////////
// Container with title text and close button
/////////////////////////////////////////////
    Item {
        id: titleBar

        property bool closeButtonVisible: true

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        
        property alias text: titleLabel.text

        height: dialogStyle.current.get("titleBarHeight")
        
        signal requestClose

        Style {
            id: titleBarStyle
            styleObjectName: "MDialogTitleBar"
            // XXX MT also uses a LabelStyle here, not sure why objectName
            // is not enough since all we get is the background
            styleClass: "MLabelStyle"
        }
        
            
    Style {
      id: dialogStyle
      styleClass: "MDialogStyle"
     }

        Background {
            anchors.fill: parent
            style: titleBarStyle
        }

        Label {
            id: titleLabel
            anchors.left: parent.left
            anchors.right: closeButton.left
            anchors.verticalCenter: parent.verticalCenter
            font.capitalization: {
                if (dialogStyle.current.get("titleCapitalization"))
                    return Font.AllUppercase
                return Font.MixedCase
            }

            horizontalAlignment: {
                var alignment = dialogStyle.current.get("titleBarAlignment");
                if (alignment == Qt.AlignCenter)
                    return Qt.AlignHCenter;
                return alignment;
            }
        }

        IconButton {
            id: closeButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: titleBarStyle.current.get("marginRight");

            iconSource: "image://theme/icon-m-framework-close"
            onClicked: titleBar.requestClose()
            visible: titleBar.closeButtonVisible
            
        }

    }
