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

    property alias dialogTitle: titleLabel.text
    property alias dialogText: contentWidget.text
    signal buttonClicked(string button)

    property int maximumHeight: 400
    property int leftMargin: dialogStyle.current.get("dialogLeftMargin")
    property int rightMargin: dialogStyle.current.get("dialogRightMargin")
    property int topMargin: dialogStyle.current.get("dialogTopMargin")
    property int bottomMargin: dialogStyle.current.get("dialogBottomMargin")
    property bool centered: dialogStyle.current.get("dialogVerticalAlignment") == Qt.AlignCenter

    function dismiss() {
        // XXX In the future this should evaluate the current buttons and
        //     simulate a click on the proper one
        root.buttonClicked("No");
    }

    implicitWidth: 500  // arbitrary
    implicitHeight: Math.min(maximumHeight, titleBar.height + contentsViewport.contentHeight)

    Style {
        id: dialogStyle
        styleClass: "MDialogStyle"
    }

    /////////////////////////////////////////////
    // Container with title text and close button
    /////////////////////////////////////////////
    Item {
        id: titleBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: dialogStyle.current.get("titleBarHeight")

        Style {
            id: titleBarStyle
            styleObjectName: "MDialogTitleBar"
            // XXX MT also uses a LabelStyle here, not sure why objectName
            // is not enough since all we get is the background
            styleClass: "MLabelStyle"
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
            styleObjectName: "MDialogTitleLabel"
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

            iconId: "icon-m-framework-close"
            styleObjectName: "MDialogCloseButton"
            onClicked: root.dismiss()
        }
    }

    ///////////////////////////////////////////////////////////////
    // Flickable with background, dialog payload and row of buttons
    ///////////////////////////////////////////////////////////////
    Style {
        id: contentsStyle
        styleClass: "MPannableWidgetStyle"
        styleObjectName: "MDialogContentsViewport"
    }

    // Should be outside the Flickable since it does not move
    Background {
        anchors.fill: contentsViewport
        style: contentsStyle
    }

    Flickable {
        id: contentsViewport

        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: titleBar.left
        anchors.right: titleBar.right

        clip: true
        contentHeight: calculateContentHeight()

        // Calculate height that can fit all label content, the button row and
        // two spacings (between label and buttons, and below the buttons)
        function calculateContentHeight() {
            var width = 0;
            width += contentWidget.height;
            width += buttons.height;
            width += dialogStyle.current.get("verticalSpacing") * 2;
            return width;
        }

        // XXX A more general solution is to provide an user-accessible way of
        //     providing this widget, rather than exporting just a text message
        Label {
            id: contentWidget
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            wrapMode: Text.Wrap
        }

        Row {
            id: buttons
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: dialogStyle.current.get("verticalSpacing")

            height: childrenRect.height
            spacing: dialogStyle.current.get("buttonSpacing")

            property int buttonWidth: calculateButtonWidth()

            function calculateButtonWidth() {
                var width = dialogStyle.current.get("dialogButtonFixedWidth")
                if (width > 0)
                    return width;

                if (buttons.children.length == 0)
                    return 0;

                width = contentsViewport.width
                width -= (buttons.children.length - 1) * buttons.spacing
                return Math.floor(width / buttons.children.length)
            }

            // XXX Make this dynamic in the future
            Button {
                text: "Yes"
                styleObjectName: "MDialogButtonOk"
                buttonType: "first"
                width: parent.buttonWidth
                onClicked: root.buttonClicked(text)
            }

            Button {
                text: "No"
                buttonType: "last"
                width: parent.buttonWidth
                onClicked: root.buttonClicked(text)
            }
        }
    }
}
