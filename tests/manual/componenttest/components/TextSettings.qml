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

import QtQuick 1.0
import com.nokia.symbian 1.0

ListView {
    id: settings

    property int fontCapitalization: Font.MixedCase
    property string fontFamily: platformStyle.fontFamilyRegular
    property real fontLetterSpacing: 0
    property int fontPixelSize: platformStyle.fontSizeMedium
    property int fontWeight: Font.Normal
    property int horizontalAlignment: TextEdit.AlignLeft
    property int inputMethodHints: 0
    property int wrapMode: TextEdit.Wrap
    property int verticalAlignment: TextEdit.AlignTop

    QtObject {
        id: privy

        function changeSetting(index, value) {
            switch (index) {
            case 0: settings.wrapMode = value; break
            case 1: settings.horizontalAlignment = value; break
            case 2: settings.verticalAlignment = value; break
            case 3: settings.fontFamily = value; break
            case 4: settings.fontCapitalization = value; break
            case 5: settings.fontLetterSpacing = value; break
            case 6: settings.fontPixelSize = value; break
            case 7: settings.fontWeight = value; break
            case 8: settings.inputMethodHints = value; break
            default: break // do nothing
            }
        }

        function createFontModel(model, submodel) {
            var fontFamiliest = Qt.fontFamilies()
            submodel.clear()
            for (var i = 1; i < fontFamiliest.length; ++i) {
                submodel.append({"name": fontFamiliest[i], "value": fontFamiliest[i]})
                if (fontFamiliest[i] == settings.fontFamily)
                    model.setProperty(3, "initialIndex", i)
            }
            return submodel
        }

        function imHintSubtitle() {
            // input method hints are at index 8
            var model = settings.model.get(8).dialogModel
            var subtitle = ""
            for (var i = 0; i < model.count; ++i) {
                if (model.get(i).selected)
                    subtitle += model.get(i).name + " "
            }
            return subtitle
        }
    }

    clip: true
    model: TextSettingsModel { }

    delegate: SelectionListItem {
        title: setting
        subTitle: {
            if (!multiSelection)
                return selectionDialog.model.get(selectionDialog.selectedIndex).name

            if (setting == "Input method hints")
                return privy.imHintSubtitle()

            return ""
        }

        onClicked: selectionDialog.open()
        onPressAndHold: selectionDialog.open()

        SelectionDialog {
            id: selectionDialog
            titleText: setting
            selectedIndex: initialIndex
            delegate: defaultDelegate
            model: {
                if (setting == "Font family")
                    return privy.createFontModel(settings.model, dialogModel)

                return dialogModel
            }

            onAccepted: privy.changeSetting(settings.currentIndex,
                                            selectionDialog.model.get(selectionDialog.selectedIndex).value)

            // Multiselection dialog is accepted when pressed outside of dialog.
            onRejected: {
                if (!multiSelection)
                    return

                privy.changeSetting(settings.currentIndex,
                                    settings.model.get(settings.currentIndex).values)
            }

            Component {
                id: defaultDelegate

                MenuItem {
                    text: name
                    onClicked: {
                        selectedIndex = index

                        // update model
                        if (setting ==  "Input method hints") {
                            checkbox.item.checked = !checkbox.item.checked
                            settings.model.get(8).dialogModel.get(index).selected = checkbox.item.checked
                            settings.model.get(8).values ^= settings.model.get(8).dialogModel.get(index).value
                        }

                        // Multiselection dialog is accepted when pressed outside of dialog.
                        if (!multiSelection)
                            root.accept()
                    }

                    Loader {
                        id: checkbox
                        sourceComponent: multiSelection ? checkBoxComponent : undefined

                        anchors {
                            right: parent.right
                            rightMargin: privateStyle.scrollBarThickness
                            verticalCenter: parent.verticalCenter
                        }
                    }

                    Component {
                        id: checkBoxComponent
                        CheckBox {
                            checked: selected
                            onClicked: {
                                settings.model.get(8).dialogModel.get(index).selected = checkbox.item.checked
                                settings.model.get(8).values ^= settings.model.get(8).dialogModel.get(index).value
                            }
                        }
                    }
                }
            }
        }
    }
    ScrollBar {
        flickableItem: settings
        anchors { top: settings.top; right: settings.right }
        interactive: false
    }
}
