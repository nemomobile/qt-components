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
#include <QTest>
#include <QSignalSpy>
#include <QDeclarativeView>
#include <QDeclarativeItem>

class tst_ApplicationWindow : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void contentArea();

private:
    QScopedPointer<QDeclarativeView> view;
    QGraphicsObject *applicationWindow;
    QGraphicsObject *content;
};


void tst_ApplicationWindow::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_applicationwindow.qml"));
    view->show();
    view->activateWindow();
    QTest::qWaitForWindowShown(view.data());
    QVERIFY(view);
    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));

    applicationWindow = view->rootObject();
    QVERIFY(applicationWindow);
    QVERIFY(!applicationWindow->property("fullScreen").toBool());

    QVERIFY(applicationWindow->property("pageStack").canConvert<QDeclarativeItem *>());
    QDeclarativeItem *pageStack = applicationWindow->property("pageStack").value<QDeclarativeItem *>();
    QVERIFY(pageStack);

    content = applicationWindow->findChild<QGraphicsObject*>("page");
    QVERIFY(content);
}

void tst_ApplicationWindow::contentArea()
{
    QCOMPARE(content->scenePos().x(), 0.0);
    QVERIFY(content->scenePos().y() > 0.0);
    applicationWindow->setProperty("fullScreen", true);
    QCoreApplication::sendPostedEvents();
    QTRY_COMPARE(content->scenePos().y(), 0.0);
}

QTEST_MAIN(tst_ApplicationWindow)

#include "tst_applicationwindow.moc"
