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

#include "tst_quickcomponentstest.h"
#include "sdeclarativescreen.h"
#include <QTest>
#include <QSignalSpy>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

class tst_popupmanager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void popupStackDepth();

private:
    QScopedPointer<QDeclarativeView> view;
    QGraphicsObject *window;
};

void tst_popupmanager::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_popupmanager.qml"));
    view->show();
    QVERIFY(view.data());
    QTest::qWaitForWindowShown(view.data());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));
    window = view->rootObject();
    QVERIFY(window);
    window->setProperty("openDialogs", true);
}

void tst_popupmanager::popupStackDepth()
{
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 3);
    window->setProperty("dialogAmount", 2);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 2);
    window->setProperty("dialogAmount", 1);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 1);
    window->setProperty("dialogAmount", 0);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 0);
}

QTEST_MAIN(tst_popupmanager)

#include "tst_popupmanager.moc"
