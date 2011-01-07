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
import "UIConstants.js" as UI

ImplicitSizeItem {
    id: button

    // Common public API
    property alias checked: checkable.checked
    property alias checkable: checkable.enabled
    property alias pressed: mouseArea.pressed
    property alias text: label.text
    property url iconSource

    signal clicked

    // Defines the button viewtype. Usually this is automatically set by
    // specialized containers like the ButtonRow or the QueryDialog
    property string buttonType
    property string groupPosition

    implicitWidth: calculateWidth()
    implicitHeight: 50

    property alias font: label.font

    // Internal property, to be used by ButtonRow and other button containers.
    property alias __exclusiveGroup: checkable.exclusiveGroup

    function calculateWidth() {
        // XXX Check how does MeeGo Touch does that. Maybe use style paddings, etc
        var prefWidth = 32;

        if (label.visible && iconFromSource.visible) {
            prefWidth += 1 + labelSizeHelper.width + iconFromSource.width + 10;
        } else if (label.visible) {
            prefWidth += 1 + labelSizeHelper.width;
        } else if (iconFromSource.visible) {
            prefWidth += iconFromSource.width;
        }

        return prefWidth;
    }

    BorderImage {
        id: background
        anchors.fill: parent
        border { left: UI.CORNER_MARGINS; top: UI.CORNER_MARGINS; right: UI.CORNER_MARGINS; bottom: UI.CORNER_MARGINS }
        source: getImageName()

        function getImageName() {
            var name = "image://theme/meegotouch-button";

            if (button.buttonType == "affirmative")
                name += "-positive";
            else if (button.buttonType == "negative")
                name += "-negative";

            name += "-background";

            if (!enabled)
                name += "-disabled";
            else if (mouseArea.containsMouse && mouseArea.pressed)
                name += "-pressed";
            else if (checkable.checked)
                name += "-selected"

            if (button.buttonType == "group" && button.groupPosition != "")
                name += "-" + button.groupPosition;
            return name;
        }
    }

    Image {
        id: iconFromSource
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: calculateContentMargin()
        sourceSize.width: UI.SIZE_ICON_DEFAULT
        sourceSize.height: UI.SIZE_ICON_DEFAULT

        source: button.iconSource

        // If button is wider than content, grow the margins to keep content centered
        function calculateContentMargin() {
            var margin = UI.MARGIN_XLARGE;
            if (button.width > button.implicitWidth)
                margin += (button.width - button.implicitWidth) / 2;
            return margin;
        }

        // Visibility check for default state (icon is not explicitly hidden)
        visible: source != ""
    }

    Label {
        id: label
        anchors.left: iconFromSource.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: marginRespectingIconWidth()
        anchors.rightMargin: iconFromSource.anchors.leftMargin

        // Label left margin should be large enough to leave space for the icon
        // when we have one
        function marginRespectingIconWidth() {
            if (iconFromSource.visible)
                return iconFromSource.width + 10;
            return 0;
        }

        elide: Text.ElideRight

        // XXX This does not make sense yet, since the label width is not being set
        // horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        font.family: UI.FONT_FAMILY
        font.weight: Font.Normal
        font.pixelSize: UI.FONT_DEFAULT_SIZE
        color: getFontColor()

        function getFontColor() {
            if (!enabled)
                return UI.COLOR_DISABLED_FOREGROUND
            else if (buttonType == "affirmative" || buttonType == "negative")
                return mouseArea.containsMouse && mouseArea.pressed ? UI.COLOR_INVERTED_SECONDARY_FOREGROUND : UI.COLOR_INVERTED_FOREGROUND
            else
                return (mouseArea.containsMouse && mouseArea.pressed) || (button.checkable && button.checked) ?
                    UI.COLOR_INVERTED_FOREGROUND : UI.COLOR_FOREGROUND
        }
        text: ""

        visible: text != ""
    }

    // This invisible label is used to provide the required width to fit the current label text
    // XXX remove when Label provide this by itself
    Text {
        id: labelSizeHelper

        font: label.font
        text: label.text

        visible: false
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: {
            //XXX Must have a new way to use the feedback from QML, due to the themebridge dependency removal
            //meegostyle.feedback("pressFeedback");
        }

        onClicked: {
            //XXX Must have a new way to use the feedback from QML, due to the themebridge dependency removal
            //meegostyle.feedback("releaseFeedback");
            checkable.toggle();
            button.clicked();
        }
    }

    Checkable {
        id: checkable
        value: button
    }
}
