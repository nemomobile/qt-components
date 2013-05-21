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

#include <QtTest/QtTest>
#include <qdeclarativeview.h>
#include <qdeclarativeengine.h>

#include "slider/apicheck_slider.h"
#include "button/apicheck_button.h"
#include "checkbox/apicheck_checkbox.h"
#include "dialog/apicheck_dialog.h"
#include "infobanner/apicheck_infobanner.h"
#include "radiobutton/apicheck_radiobutton.h"
#include "ratingindicator/apicheck_ratingindicator.h"
#include "textfield/apicheck_textfield.h"
#include "progressbar/apicheck_progressbar.h"
#include "tabbutton/apicheck_tabbutton.h"
#include "tabgroup/apicheck_tabgroup.h"
#include "textarea/apicheck_textarea.h"
#include "scrolldecorator/apicheck_scrolldecorator.h"
#include "window/apicheck_window.h"
#include "page/apicheck_page.h"
#include "pagestack/apicheck_pagestack.h"
#include "sectionscroller/apicheck_sectionscroller.h"
#include "busyindicator/apicheck_busyindicator.h"
#include "buttoncolumn/apicheck_buttoncolumn.h"
#include "buttonrow/apicheck_buttonrow.h"
#include "contextmenu/apicheck_contextmenu.h"
#include "menu/apicheck_menu.h"
#include "menuitem/apicheck_menuitem.h"
#include "querydialog/apicheck_querydialog.h"
#include "selectiondialog/apicheck_selectiondialog.h"
#include "toolbar/apicheck_toolbar.h"
#include "switch/apicheck_switch.h"
#include "screen/apicheck_screen.h"
#include "tumbler/apicheck_tumbler.h"
#include "tumbler/apicheck_tumblercolumn.h"
#include "tumblerdialog/apicheck_tumblerdialog.h"
#include "datepickerdialog/apicheck_datepickerdialog.h"
#include "timepickerdialog/apicheck_timepickerdialog.h"
#include "searchbox/apicheck_searchbox.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QStringList args = app.arguments();

    QDeclarativeView view;
    QDeclarativeEngine *engine = view.engine();

    // check for module
    QString module("Qt.labs.components.native 1.0");
    int moduleIdx = args.indexOf("-module");

    if (moduleIdx != -1) {
        args.removeAt(moduleIdx);
        module = args.takeAt(moduleIdx++);
    }

    // check for import path
    QString importPath(Q_COMPONENTS_BUILD_TREE"/imports");
    int pathIdx = args.indexOf("-importpath");

    if (pathIdx != -1) {
        args.removeAt(pathIdx);
        importPath = args.takeAt(pathIdx++);
    }

    // setup engine's import path
    engine->addImportPath(QDir(importPath).canonicalPath());    //QTBUG-16885

    // create tests
    ApiCheckSlider slider(engine, module);
    ApiCheckButton button(engine, module);
    ApiCheckButtonColumn buttonColumn(engine, module);
    ApiCheckButtonRow buttonRow(engine, module);
    ApiCheckCheckBox checkbox(engine, module);
    ApiCheckDialog dialog(engine, module);
    ApiCheckInfoBanner infoBanner(engine, module);
    ApiCheckTextField textField(engine, module);
    ApiCheckRadioButton radioButton(engine, module);
    ApiCheckProgressBar progressBar(engine, module);
    ApiCheckTabButton tabButton(engine, module);
    ApiCheckTabGroup tabGroup(engine, module);
    ApiCheckTextArea textArea(engine, module);
    ApiCheckToolBar toolBar(engine, module);
    ApiCheckScrollDecorator scrollDecorator(engine, module);
    ApiCheckWindow window(engine, module);
    ApiCheckPage page(engine, module);
    ApiCheckPageStack pageStack(engine, module);
    ApiCheckSectionScroller sectionScroller(engine, module);
    ApiCheckBusyIndicator busyIndicator(engine, module);
    ApiCheckContextMenu contextMenu(engine, module);
    ApiCheckMenu menu(engine, module);
    ApiCheckMenuItem menuItem(engine, module);
    ApiCheckRatingIndicator ratingIndicator(engine, module);
    ApiCheckSwitch switchButton(engine, module);
    ApiCheckScreen screen(engine, module);
    ApiCheckTumbler tumbler(engine, module);
    ApiCheckTumblerColumn tumblerColumn(engine, module);
    ApiCheckTumblerDialog tumblerDialog(engine, module);
    ApiCheckQueryDialog querydialog(engine, module);
    ApiCheckSelectionDialog selectiondialog(engine, module);
    ApiCheckDatePickerDialog datepickerdialog(engine, module);
    ApiCheckTimePickerDialog timepickerdialog(engine, module);
    ApiCheckSearchBox searchbox(engine, module);

    QStringList failures;
    if (QTest::qExec(&slider, args)) failures.append("slider");
    if (QTest::qExec(&button, args)) failures.append("button");
    if (QTest::qExec(&buttonColumn, args)) failures.append("buttonColumn");
    if (QTest::qExec(&buttonRow, args)) failures.append("buttonRow");
    if (QTest::qExec(&checkbox, args)) failures.append("checkbox");
    if (QTest::qExec(&dialog, args)) failures.append("dialog");
    if (QTest::qExec(&infoBanner, args)) failures.append("infoBanner");
    if (QTest::qExec(&textField, args)) failures.append("textField");
    if (QTest::qExec(&radioButton, args)) failures.append("radioButton");
    if (QTest::qExec(&progressBar, args)) failures.append("progressBar");
    if (QTest::qExec(&tabButton, args)) failures.append("tabButton");
    if (QTest::qExec(&tabGroup, args)) failures.append("tabGroup");
    if (QTest::qExec(&toolBar, argc)) failures.append("toolBar");
    if (QTest::qExec(&textArea, args)) failures.append("textArea");
    if (QTest::qExec(&scrollDecorator, args)) failures.append("scrollDecorator");
    if (QTest::qExec(&window, args)) failures.append("window");
    if (QTest::qExec(&page, argc)) failures.append("page");
    if (QTest::qExec(&pageStack, argc)) failures.append("pageStack");
    if (QTest::qExec(&sectionScroller, args)) failures.append("sectionScroller");
    if (QTest::qExec(&busyIndicator, args)) failures.append("busyIndicator");
    if (QTest::qExec(&contextMenu, argc)) failures.append("contextMenu");
    if (QTest::qExec(&menu, argc)) failures.append("menu");
    if (QTest::qExec(&menuItem, args)) failures.append("menuItem");
    if (QTest::qExec(&ratingIndicator, args)) failures.append("ratingIndicator");
    if (QTest::qExec(&switchButton, argc)) failures.append("switchButton");
    if (QTest::qExec(&screen, argc)) failures.append("screen");
    if (QTest::qExec(&tumbler, args)) failures.append("tumbler");
    if (QTest::qExec(&tumblerColumn, args)) failures.append("tumblerColumn");
    if (QTest::qExec(&tumblerDialog, args)) failures.append("tumblerDialog");
    if (QTest::qExec(&querydialog, args)) failures.append("querydialog");
    if (QTest::qExec(&selectiondialog, args)) failures.append("selectiondialog");
    if (QTest::qExec(&datepickerdialog, args)) failures.append("datepickerdialog");
    if (QTest::qExec(&timepickerdialog, args)) failures.append("timepickerdialog");
    if (QTest::qExec(&searchbox, args)) failures.append("searchbox");

    if (!failures.isEmpty())
        qWarning("FAILED : %s", qPrintable(failures.join(", ")));
    return failures.count();
}
