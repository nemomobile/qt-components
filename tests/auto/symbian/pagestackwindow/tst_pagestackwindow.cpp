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
#include <QDeclarativeContext>

// Test that the condition is fulfilled,
// retry and wait max 10 seconds before failure.
// This does not cause unnecessary waits to make tests longer in normal conditions,
// but still does not cause false failures if e.g. updating the screen takes longer
// than normally.
#define VERIFY_WITH_WAIT_LOOP(a) \
    {bool status = false;\
    for (int i=0; i<100; ++i) {\
        if (a) {\
            status = true;\
            break;\
        }\
        QTest::qWait(100);\
    }\
    QVERIFY(status);}

class tst_PageStackWindow : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void contentArea();
    void softwareInputPanel();

private:
    QScopedPointer<QDeclarativeView> view;
    QGraphicsObject *pageStackWindow;
    QGraphicsObject *content;
};


void tst_PageStackWindow::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_pagestackwindow.qml"));
    view->show();
    view->activateWindow();
    QTest::qWaitForWindowShown(view.data());
    QVERIFY(view);
    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));

    pageStackWindow = view->rootObject();
    QVERIFY(pageStackWindow);
    QVERIFY(pageStackWindow->property("showStatusBar").toBool());
    QVERIFY(pageStackWindow->property("showToolBar").toBool());
    QVERIFY(!pageStackWindow->property("platformSoftwareInputPanelEnabled").toBool());

    QVERIFY(pageStackWindow->property("pageStack").canConvert<QDeclarativeItem *>());
    QDeclarativeItem *pageStack = pageStackWindow->property("pageStack").value<QDeclarativeItem *>();
    QVERIFY(pageStack);

    content = pageStackWindow->findChild<QGraphicsObject*>("page");
    QVERIFY(content);
}

void tst_PageStackWindow::contentArea()
{
    QCOMPARE(content->scenePos().x(), 0.0);
    QVERIFY(content->scenePos().y() > 0.0);
    qreal height = content->boundingRect().height();
    // Set statusbar invisible
    pageStackWindow->setProperty("showStatusBar", false);

    VERIFY_WITH_WAIT_LOOP(content->scenePos().y() == 0.0);
    QVERIFY(content->boundingRect().height() > height);
    height = content->boundingRect().height();
    // Set toolbar invisible
    pageStackWindow->setProperty("showToolBar", false);
    // Toolbar does not affect content item's height
    VERIFY_WITH_WAIT_LOOP(content->boundingRect().height() == height);
    height = content->boundingRect().height();

    pageStackWindow->setProperty("showStatusBar", true);
    pageStackWindow->setProperty("showToolBar", true);
}

void tst_PageStackWindow::softwareInputPanel()
{
    QVERIFY(!pageStackWindow->property("platformSoftwareInputPanelEnabled").toBool());
}

QTEST_MAIN(tst_PageStackWindow)

#include "tst_pagestackwindow.moc"
