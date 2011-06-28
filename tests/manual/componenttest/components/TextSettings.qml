/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import com.nokia.symbian 1.1

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
    property bool platformInverted: false

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
        platformInverted: settings.platformInverted

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
            platformInverted: settings.platformInverted

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
                    platformInverted: settings.platformInverted
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
                            platformInverted: settings.platformInverted
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
