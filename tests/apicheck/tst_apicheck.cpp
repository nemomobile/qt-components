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

#include <QtTest/QtTest>
#include <QtDeclarative/qdeclarativeview.h>
#include <QtDeclarative/qdeclarativeengine.h>

#include "slider/apicheck_slider.h"
#include "button/apicheck_button.h"
#include "checkbox/apicheck_checkbox.h"
#include "dialog/apicheck_dialog.h"
#include "radiobutton/apicheck_radiobutton.h"
#include "ratingindicator/apicheck_ratingindicator.h"
#include "textfield/apicheck_textfield.h"
#include "progressbar/apicheck_progressbar.h"
#include "tabbutton/apicheck_tabbutton.h"
#include "tabgroup/apicheck_tabgroup.h"
#include "textarea/apicheck_textarea.h"
#include "scrolldecorator/apicheck_scrolldecorator.h"
#include "choicelist/apicheck_choicelist.h"
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

    // extras module
    QString extrasModule("com.nokia.extras 1.0");
    int extrasModuleIdx = args.indexOf("-extrasModule");

    if (extrasModuleIdx != -1) {
        args.removeAt(extrasModuleIdx);
        extrasModule = args.takeAt(extrasModuleIdx++);
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
    ApiCheckTextField textField(engine, module);
    ApiCheckRadioButton radioButton(engine, module);
    ApiCheckProgressBar progressBar(engine, module);
    ApiCheckTabButton tabButton(engine, module);
    ApiCheckTabGroup tabGroup(engine, module);
    ApiCheckTextArea textArea(engine, module);
    ApiCheckToolBar toolBar(engine, module);
    ApiCheckScrollDecorator scrollDecorator(engine, module);
    ApiCheckChoiceList choiceList(engine, module);
    ApiCheckWindow window(engine, module);
    ApiCheckPage page(engine, module);
    ApiCheckPageStack pageStack(engine, module);
    ApiCheckSectionScroller sectionScroller(engine, module);
    ApiCheckBusyIndicator busyIndicator(engine, module);
    ApiCheckContextMenu contextMenu(engine, module);
    ApiCheckMenu menu(engine, module);
    ApiCheckMenuItem menuItem(engine, module);
    ApiCheckRatingIndicator ratingIndicator(engine, extrasModule);
    ApiCheckSwitch switchButton(engine, module);
    ApiCheckScreen screen(engine, module);
    ApiCheckTumbler tumbler(engine, extrasModule);
    ApiCheckTumblerColumn tumblerColumn(engine, extrasModule);
    ApiCheckTumblerDialog tumblerDialog(engine, extrasModule);
    ApiCheckQueryDialog querydialog(engine, module);
    ApiCheckSelectionDialog selectiondialog(engine, module);
    ApiCheckDatePickerDialog datepickerdialog(engine, extrasModule);

    int ret = 0;
    ret |= QTest::qExec(&slider, args);
    ret |= QTest::qExec(&button, args);
    ret |= QTest::qExec(&buttonColumn, args);
    ret |= QTest::qExec(&buttonRow, args);
    ret |= QTest::qExec(&checkbox, args);
    ret |= QTest::qExec(&dialog, args);
    ret |= QTest::qExec(&textField, args);
    ret |= QTest::qExec(&radioButton, args);
    ret |= QTest::qExec(&progressBar, args);
    ret |= QTest::qExec(&tabButton, args);
    ret |= QTest::qExec(&tabGroup, args);
    ret |= QTest::qExec(&toolBar, argc);
    ret |= QTest::qExec(&textArea, args);
    ret |= QTest::qExec(&scrollDecorator, args);
    ret |= QTest::qExec(&choiceList, args);
    ret |= QTest::qExec(&window, args);
    ret |= QTest::qExec(&page, argc);
    ret |= QTest::qExec(&pageStack, argc);
    ret |= QTest::qExec(&sectionScroller, args);
    ret |= QTest::qExec(&busyIndicator, args);
    ret |= QTest::qExec(&contextMenu, argc);
    ret |= QTest::qExec(&menu, argc);
    ret |= QTest::qExec(&menuItem, args);
    ret |= QTest::qExec(&ratingIndicator, args);
    ret |= QTest::qExec(&switchButton, argc);
    ret |= QTest::qExec(&screen, argc);
    ret |= QTest::qExec(&tumbler, args);
    ret |= QTest::qExec(&tumblerColumn, args);
    ret |= QTest::qExec(&tumblerDialog, args);
    ret |= QTest::qExec(&querydialog, args);
    ret |= QTest::qExec(&selectiondialog, args);
    ret |= QTest::qExec(&datepickerdialog, args);

    return ret;
}
