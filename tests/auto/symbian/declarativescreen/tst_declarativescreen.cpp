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

#include "sdeclarativescreen.h"
#include "tst_quickcomponentstest.h"
#include <QTest>
#include <QUrl>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QMainWindow>
#include <QSignalSpy>

class tst_SDeclarativeScreen : public QObject
{
    Q_OBJECT
private slots:

    void initTestCase();
    void defaults();
    void changeOrientation();
    void changeScreenSize();
    void startupOrientation();
    void twoDeclarativeViews();
    void graphicsView();

private:

    void initView(const QString &fileName);
    QObject *screen;
    QScopedPointer<QDeclarativeView> view;
    QScopedPointer<QObject> applicationWindow;
};

void tst_SDeclarativeScreen::initTestCase()
{
    initView("tst_declarativescreen.qml");
}

void tst_SDeclarativeScreen::defaults()
{
    QVERIFY(screen->property("width").toInt() > 0);
    QVERIFY(screen->property("height").toInt() > 0);
    QVERIFY(screen->property("displayWidth").toInt() > 0);
    QVERIFY(screen->property("displayHeight").toInt() > 0);
    QVERIFY(screen->property("dpi").toDouble() > (double)0);
    QCOMPARE(screen->property("allowedOrientations").toInt(), (int)SDeclarativeScreen::Default);

    if (screen->property("currentOrientation").toInt() == SDeclarativeScreen::Portrait
        || (screen->property("currentOrientation").toInt() == SDeclarativeScreen::PortraitInverted)) {
        QVERIFY(screen->property("width").toInt() < screen->property("height").toInt());
    } else if (screen->property("currentOrientation").toInt() == SDeclarativeScreen::Landscape
        || screen->property("currentOrientation").toInt() == SDeclarativeScreen::LandscapeInverted) {
        QVERIFY(screen->property("height").toInt() < screen->property("width").toInt());
    }
}

void tst_SDeclarativeScreen::changeOrientation()
{
    QSignalSpy currentOrientationSpy(screen, SIGNAL(currentOrientationChanged()));
    QSignalSpy allowedOrientationsSpy(screen, SIGNAL(allowedOrientationsChanged()));
    QSignalSpy widthSpy(screen, SIGNAL(widthChanged()));
    QSignalSpy heightSpy(screen, SIGNAL(heightChanged()));
    QSignalSpy displaySpy(screen, SIGNAL(displayChanged()));

    int displayWidth = screen->property("displayWidth").toInt();
    int displayHeight = screen->property("displayHeight").toInt();

    if (screen->property("currentOrientation").toInt() == SDeclarativeScreen::Portrait
        || screen->property("currentOrientation").toInt() == SDeclarativeScreen::PortraitInverted) {
        screen->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
        QCOMPARE(screen->property("allowedOrientations").toInt(), (int)SDeclarativeScreen::Landscape);
        QVERIFY(screen->property("width").toInt() > screen->property("height").toInt());
    } else {
        screen->setProperty("allowedOrientations", SDeclarativeScreen::Portrait);
        QCOMPARE(screen->property("allowedOrientations").toInt(), (int)SDeclarativeScreen::Portrait);
        QVERIFY(screen->property("height").toInt() > screen->property("width").toInt());
    }

    // ensure that resizeEvent gets handled
    QApplication::sendPostedEvents();

    // ensure that display size has not changed
    QCOMPARE(screen->property("displayWidth").toInt(), displayWidth);
    QCOMPARE(screen->property("displayHeight").toInt(), displayHeight);

    QCOMPARE(currentOrientationSpy.count(), 1);
    QCOMPARE(allowedOrientationsSpy.count(), 1);
    QCOMPARE(widthSpy.count(), 1);
    QCOMPARE(heightSpy.count(), 1);
    QCOMPARE(displaySpy.count(), 0); // no "displayChanged" signal
}

void tst_SDeclarativeScreen::changeScreenSize()
{
    QMetaObject::invokeMethod(screen, "privateSetDisplay",
                              Q_ARG(int, 360),
                              Q_ARG(int, 640),
                              Q_ARG(qreal, 200));
    QCOMPARE(screen->property("width").toInt(), 360);
    QCOMPARE(screen->property("height").toInt(), 640);
    QCOMPARE(screen->property("currentOrientation").toInt(), (int)SDeclarativeScreen::Portrait);
    QCOMPARE(screen->property("dpi").toDouble(), (double)200);
    QCOMPARE(screen->property("displayCategory").toInt(), (int)SDeclarativeScreen::Normal);
    QCOMPARE(screen->property("density").toInt(), (int)SDeclarativeScreen::High);
    QMetaObject::invokeMethod(screen, "privateSetDisplay",
                              Q_ARG(int, 640),
                              Q_ARG(int, 360),
                              Q_ARG(qreal, 120));
    QCOMPARE(screen->property("width").toInt(), 640);
    QCOMPARE(screen->property("height").toInt(), 360);
    QCOMPARE(screen->property("currentOrientation").toInt(), (int)SDeclarativeScreen::Landscape);
    QCOMPARE(screen->property("dpi").toDouble(), (double)120);
    QCOMPARE(screen->property("displayCategory").toInt(), (int)SDeclarativeScreen::Large);
    QCOMPARE(screen->property("density").toInt(), (int)SDeclarativeScreen::Low);
}

void tst_SDeclarativeScreen::startupOrientation()
{
    initView("tst_declarativescreen2.qml");
    QGraphicsObject* applicationWindow = view->rootObject();
    QVERIFY(applicationWindow);
    QVERIFY(applicationWindow->property("width").toInt() > 0);
    QVERIFY(applicationWindow->property("height").toInt() > 0);
    QVERIFY(applicationWindow->property("width").toInt() > applicationWindow->property("height").toInt());
    QCOMPARE(screen->property("currentOrientation").toInt(), (int)SDeclarativeScreen::Landscape);
}

void tst_SDeclarativeScreen::initView(const QString &fileName)
{
    if (view.isNull())
        view.reset(tst_quickcomponentstest::createDeclarativeView(fileName));
    QVERIFY(view);
    QDeclarativeView *v = view.data();
    QString errors;
    applicationWindow.reset(tst_quickcomponentstest::createComponentFromFile(fileName, &errors, &v));
    QVERIFY2(applicationWindow, qPrintable(errors));

    view->activateWindow();
    QApplication::setActiveWindow(view.data());
    view->show();
    QTest::qWaitForWindowShown(view.data());

    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));
    screen = qVariantValue<QObject *>(view->engine()->rootContext()->contextProperty("screen"));
    QVERIFY(screen);
}

void tst_SDeclarativeScreen::twoDeclarativeViews()
{
    QDeclarativeView *view2 = tst_quickcomponentstest::createDeclarativeView("tst_declarativescreen.qml");
    QVERIFY(view2);

    // set another top level widget as parent
    QScopedPointer<QMainWindow> rootWindow(new QMainWindow);
    view2->setParent(rootWindow.data());
    rootWindow->setGeometry(view2->x() + view2->width() + 20, view->y(), 100, 100);
    view2->setGeometry(0,0,rootWindow->width(), rootWindow->height());

    QString errors;
    QScopedPointer<QObject> applicationWindow2(
        tst_quickcomponentstest::createComponentFromFile("tst_declarativescreen.qml", &errors, &view2));
    QVERIFY2(applicationWindow2.data(), qPrintable(errors));

    rootWindow->activateWindow();
    QApplication::setActiveWindow(rootWindow.data());
    rootWindow->show();
    QTest::qWaitForWindowShown(rootWindow.data());

    QVERIFY(view2->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(rootWindow.data()));
    QObject *screen2 = qVariantValue<QObject *>(view2->engine()->rootContext()->contextProperty("screen"));
    QVERIFY(screen2);

    // screen1 portrait, screen2 landscape
    screen->setProperty("allowedOrientations", SDeclarativeScreen::Portrait);
    screen2->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
    QTRY_VERIFY(applicationWindow->property("width").toReal() < applicationWindow->property("height").toReal());
    QTRY_VERIFY(applicationWindow2->property("width").toReal() > applicationWindow2->property("height").toReal());

    // screen1 landscape, screen2 portrait
    screen2->setProperty("allowedOrientations", SDeclarativeScreen::Portrait);
    screen->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
    QTRY_VERIFY(applicationWindow2->property("width").toReal() < applicationWindow2->property("height").toReal());
    QTRY_VERIFY(applicationWindow->property("width").toReal() > applicationWindow->property("height").toReal());
}

void tst_SDeclarativeScreen::graphicsView()
{
    QScopedPointer<QGraphicsView> graphicsView(new QGraphicsView());
    QGraphicsScene *scene = new QGraphicsScene(graphicsView.data());
    graphicsView->setScene(scene);
    QDeclarativeEngine *engine = new QDeclarativeEngine(graphicsView.data());
    engine->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");

    QDeclarativeComponent *component = new QDeclarativeComponent(engine);
    QFile file("tst_declarativescreen.qml");
    file.open(QFile::ReadOnly);
    component->setData(file.readAll(), QUrl());
    QObject* out = component->create();
    QVERIFY(out);
    QDeclarativeItem *rootItem = qobject_cast<QDeclarativeItem *>(out);
    QVERIFY(rootItem);
    file.close();

    scene->addItem(rootItem);
    graphicsView->show();
    QApplication::setActiveWindow(graphicsView.data());

    QTest::qWaitForWindowShown(graphicsView.data());

    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(graphicsView.data()));
    QObject *screen2 = qVariantValue<QObject *>(engine->rootContext()->contextProperty("screen"));
    QVERIFY(screen2);

    // set portrait
    screen2->setProperty("allowedOrientations", SDeclarativeScreen::Portrait);
    QVERIFY(screen2->property("width").toInt() < screen2->property("height").toInt());

    // set landscape
    screen2->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
    QVERIFY(screen2->property("width").toInt() > screen2->property("height").toInt());
}

QTEST_MAIN(tst_SDeclarativeScreen)

#include "tst_declarativescreen.moc"
