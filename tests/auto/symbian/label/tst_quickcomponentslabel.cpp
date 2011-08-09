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
#include <QGraphicsObject>
#include <QDeclarativeContext>
#include <QDeclarativeView>

class tst_quickcomponentslabel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void properties();
    void inversion();

private:
    QGraphicsObject *label;
    QScopedPointer<QDeclarativeView> view;
    QObject *platformStyle;
};

void tst_quickcomponentslabel::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_quickcomponentslabel.qml"));
    QVERIFY(view);
    view->show();
    view->activateWindow();
    QTest::qWaitForWindowShown(view.data());
    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));
    label = view->rootObject()->findChild<QGraphicsObject*>("label");
    QVERIFY(label);
    platformStyle = view->engine()->rootContext()->contextProperty("platformStyle").value<QObject*>();
    QVERIFY(platformStyle);
}

void tst_quickcomponentslabel::properties()
{
    QVERIFY(label->property("platformInverted").isValid());
    QVERIFY(!label->property("platformInverted").toBool());

    QVERIFY(label->property("color").isValid());
    QColor color = label->property("color").value<QColor>();
    QColor defaultColor = platformStyle->property("colorNormalLight").value<QColor>();
    QVERIFY(color.isValid());
    QVERIFY(defaultColor.isValid());
    QCOMPARE(color.rgb(), defaultColor.rgb());

    QVERIFY(label->property("font").isValid());
    QFont font = label->property("font").value<QFont>();
    QCOMPARE(font.family(), platformStyle->property("fontFamilyRegular").toString());
    QCOMPARE(font.pixelSize(), platformStyle->property("fontSizeMedium").toInt());
}

void tst_quickcomponentslabel::inversion()
{
    QVERIFY(label->setProperty("platformInverted", true));
    QVERIFY(label->property("platformInverted").toBool());
    QColor color = label->property("color").value<QColor>();
    QColor invertedColor = platformStyle->property("colorNormalLightInverted").value<QColor>();
    QVERIFY(color.isValid());
    QVERIFY(invertedColor.isValid());
    QCOMPARE(color.rgb(), invertedColor.rgb());
}

QTEST_MAIN(tst_quickcomponentslabel)

#include "tst_quickcomponentslabel.moc"
