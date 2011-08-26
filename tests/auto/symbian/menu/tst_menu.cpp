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

#include <QTest>
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDebug>
#include <QSignalSpy>
#include "tst_quickcomponentstest.h"

class tst_menu : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void defaultPropertyValues();
    void properties();
    void openAndClose();
    void scrollBarVisibility();

private:
    QDeclarativeView *view;
    QObject *componentObject;
    QDeclarativeItem *menu;
};

void tst_menu::initTestCase()
{
    QString errors;
    view = 0;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_menu.qml", &errors, &view);
    QVERIFY2(componentObject, qPrintable(errors));

    menu = componentObject->findChild<QDeclarativeItem*>("menu");
    QVERIFY(menu);

    view->show();
    QTest::qWaitForWindowShown(view);
}

void tst_menu::cleanupTestCase()
{
}

void tst_menu::init()
{
}

void tst_menu::cleanup()
{
}

void tst_menu::defaultPropertyValues()
{
    QVERIFY(menu->property("platformInverted").isValid());
    QCOMPARE(menu->property("platformInverted").toBool(), false);
}


void tst_menu::properties()
{
    QVERIFY(menu->setProperty("platformInverted", true));
    QCOMPARE(menu->property("platformInverted").toBool(), true);
}

void tst_menu::openAndClose()
{
    // open
    QCOMPARE(menu->property("status").toInt(), 3); // DialogStatus.Closed
    QMetaObject::invokeMethod(menu, "open");
    QCOMPARE(menu->property("status").toInt(), 0); // DialogStatus.Opening
    QTRY_COMPARE(menu->property("status").toInt(), 1); // DialogStatus.Open

    // close
    QMetaObject::invokeMethod(menu, "close");
    QCOMPARE(menu->property("status").toInt(), 2); // DialogStatus.Closing
    QTRY_COMPARE(menu->property("status").toInt(), 3); // DialogStatus.Closed
}

void tst_menu::scrollBarVisibility()
{
    QDeclarativeItem *scrollBar = componentObject->findChild<QDeclarativeItem*>("scrollBar");
    QVERIFY(scrollBar);

    // open - scrollbar should be visible for a while
    QMetaObject::invokeMethod(menu, "open");
    QTRY_COMPARE(menu->property("status").toInt(), 1); // DialogStatus.Open
    QVERIFY(scrollBar->property("opacity").toReal() > qreal(0.5));
    QTRY_VERIFY(scrollBar->property("opacity").toReal() < qreal(0.1));

    // close (just to return to non-shown state)
    QMetaObject::invokeMethod(menu, "close");
    QTRY_COMPARE(menu->property("status").toInt(), 3); // DialogStatus.Closed
}

QTEST_MAIN(tst_menu)
#include "tst_menu.moc"
