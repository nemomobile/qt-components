/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QtTest/QtTest>
#include <QUrl>
#include <QImage>
#include <QPainter>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QDeclarativeView>

class tst_SDeclarativeImageProvider : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testPng();
    void testSvg();
    void testInvalid();
private:
    QDeclarativeView *window;
    QObject *screen;
    QDeclarativeItem *image;
};

void tst_SDeclarativeImageProvider::initTestCase()
{
    window = tst_quickcomponentstest::createDeclarativeView("imagetest.qml");
    window->show();
    QTest::qWaitForWindowShown(window);

    image = 0;
    foreach (QGraphicsItem *item,  window->scene()->items()) {
        QDeclarativeItem *temp = qobject_cast<QDeclarativeItem *>(item);
        if (temp) {
            if (temp->property("source").toString().startsWith("image:")) {
                image = temp;
                break;
            }
        }
    }
    QVERIFY(image);

    screen = qVariantValue<QObject *>(window->engine()->rootContext()->contextProperty("screen"));
}

void tst_SDeclarativeImageProvider::cleanupTestCase()
{
    delete window;
}

void tst_SDeclarativeImageProvider::testPng()
{
    image->setProperty("source", QString("image://theme/list1.png"));
    QCOMPARE(image->property("width").toInt(), 100);
    QCOMPARE(image->property("height").toInt(), 100);
    QCOMPARE(image->property("sourceSize").toSize().width(), 60);
    QCOMPARE(image->property("sourceSize").toSize().height(), 60);
}

void tst_SDeclarativeImageProvider::testSvg()
{
    image->setProperty("source", QString("image://theme/image.svg"));
    QCOMPARE(image->property("width").toInt(), 100);
    QCOMPARE(image->property("height").toInt(), 100);
    QCOMPARE(image->property("sourceSize").toSize().width(), 60);
    QCOMPARE(image->property("sourceSize").toSize().height(), 12);
}

void tst_SDeclarativeImageProvider::testInvalid()
{
    image->setProperty("source", QString("image://theme/notfound.xyz"));
    QCOMPARE(image->property("width").toInt(), 100);
    QCOMPARE(image->property("height").toInt(), 100);
    QCOMPARE(image->property("sourceSize").toSize().width(), 0);
    QCOMPARE(image->property("sourceSize").toSize().height(), 0);

}
QTEST_MAIN(tst_SDeclarativeImageProvider)
#include "tst_sdeclarativeimageprovider.moc"
