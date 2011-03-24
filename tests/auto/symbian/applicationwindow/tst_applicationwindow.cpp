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
    QCOMPARE(content->scenePos().y(), 0.0);
}

QTEST_MAIN(tst_ApplicationWindow)

#include "tst_applicationwindow.moc"
