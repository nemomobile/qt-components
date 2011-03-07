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
#include <QDeclarativeItem>
#include <QDeclarativeView>
#include <QGraphicsObject>

class tst_SDeclarativeImageProvider : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void pngImage();
    void svgImage();
    void svgBorderImage();
    void invalidImage();
    void invalidBorderImage();
private:
    QScopedPointer<QDeclarativeView> view;
    QObject *image;
    QObject *borderImage;
};

void tst_SDeclarativeImageProvider::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_imageprovider.qml"));
    view->show();
    QTest::qWaitForWindowShown(view.data());
    QVERIFY(view);
    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));

    image = view->rootObject()->findChild<QObject *>("image");
    borderImage = view->rootObject()->findChild<QObject *>("borderImage");
    QVERIFY(image);
    QVERIFY(borderImage);
}

void tst_SDeclarativeImageProvider::pngImage()
{
    QCOMPARE(image->property("width").toInt(), 100);
    QCOMPARE(image->property("height").toInt(), 100);
    QCOMPARE(image->property("sourceSize").toSize().width(), 60);
    QCOMPARE(image->property("sourceSize").toSize().height(), 60);
    QVERIFY(image->property("loadOk").toBool());
}

void tst_SDeclarativeImageProvider::svgImage()
{
    image->setProperty("source", QString("image://theme/image.svg"));
    QCOMPARE(image->property("width").toInt(), 100);
    QCOMPARE(image->property("height").toInt(), 100);
    QCOMPARE(image->property("sourceSize").toSize().width(), 60);
    QCOMPARE(image->property("sourceSize").toSize().height(), 60);
    QVERIFY(image->property("loadOk").toBool());
}

void tst_SDeclarativeImageProvider::svgBorderImage()
{
    borderImage->setProperty("source", QString("image://theme/image.svg"));
    QCOMPARE(borderImage->property("width").toInt(), 200);
    QCOMPARE(borderImage->property("height").toInt(), 100);
    QVERIFY(borderImage->property("loadOk").toBool());
}

void tst_SDeclarativeImageProvider::invalidImage()
{
    image->setProperty("source", QString("image://theme/notfound.xyz"));
    QCOMPARE(image->property("width").toInt(), 100);
    QCOMPARE(image->property("height").toInt(), 100);
#if QT_VERSION < 0x040702
    QEXPECT_FAIL("", "In Qt versions < 4.7.2 sourceSize was set to 0 after invalid image source", Continue);
    QCOMPARE(image->property("sourceSize").toSize().width(), 60);
    QEXPECT_FAIL("", "In Qt versions < 4.7.2 sourceSize was set to 0 after invalid image source", Continue);
    QCOMPARE(image->property("sourceSize").toSize().height(), 60);
#else
    QCOMPARE(image->property("sourceSize").toSize().width(), 60);
    QCOMPARE(image->property("sourceSize").toSize().height(), 60);
#endif
    QVERIFY(!image->property("loadOk").toBool());
}

void tst_SDeclarativeImageProvider::invalidBorderImage()
{
    borderImage->setProperty("source", QString("image://theme/notfound.xyz"));
    QCOMPARE(borderImage->property("width").toInt(), 200);
    QCOMPARE(borderImage->property("height").toInt(), 100);
    QVERIFY(!borderImage->property("loadOk").toBool());
}

QTEST_MAIN(tst_SDeclarativeImageProvider)
#include "tst_sdeclarativeimageprovider.moc"
