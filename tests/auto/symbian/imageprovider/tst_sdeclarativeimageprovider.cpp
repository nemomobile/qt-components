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
