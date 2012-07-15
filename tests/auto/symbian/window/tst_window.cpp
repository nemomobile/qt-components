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
    void platformInverted();

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
    QCoreApplication::sendPostedEvents();
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

void tst_Window::platformInverted()
{
    QVERIFY(window->property("platformInverted").isValid());
    QCOMPARE(window->property("platformInverted").toBool(), false);
    window->setProperty("platformInverted", QVariant(true));
    QCOMPARE(window->property("platformInverted").toBool(), true);
}

QTEST_MAIN(tst_Window)

#include "tst_window.moc"
