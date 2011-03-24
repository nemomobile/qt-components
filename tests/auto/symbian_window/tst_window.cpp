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

class tst_Window : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void orientationChange();

private:
    QScopedPointer<QDeclarativeView> view;
    QObject *screen;
    QGraphicsObject *window;
};

void tst_Window::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_window.qml"));
    view->show();
    QTest::qWaitForWindowShown(view.data());
    QVERIFY(view.data());
    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));
    screen = qVariantValue<QObject *>(view->engine()->rootContext()->contextProperty("screen"));
    window = view->rootObject();
    QVERIFY(screen);
    QVERIFY(window);

    QCOMPARE(screen->property("currentOrientation").toInt(), static_cast<int>(SDeclarativeScreen::Portrait));
    QVERIFY(window->property("inPortrait").toBool());
}

void tst_Window::orientationChange()
{
    QCOMPARE(screen->property("currentOrientation").toInt(), static_cast<int>(SDeclarativeScreen::Portrait));
    QVERIFY(!window->property("inLandscape").toBool());
    QVERIFY(window->property("inPortrait").toBool());

    QSignalSpy spy1(window, SIGNAL(orientationChangeAboutToStart()));
    QSignalSpy spy2(window, SIGNAL(orientationChangeStarted()));
    QSignalSpy spy3(window, SIGNAL(orientationChangeFinished()));

    screen->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
    QCoreApplication::sendPostedEvents();

    QTRY_COMPARE(spy1.count(), 1);
    QTRY_COMPARE(spy2.count(), 1);
    QTRY_COMPARE(spy3.count(), 1);

    QVERIFY(!window->property("inPortrait").toBool());
}

QTEST_MAIN(tst_Window)

#include "tst_window.moc"
