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
#include <QDeclarativeItem>

class tst_commondialog : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void titleText();
    void titleIcon();
    void buttonTexts();
    void buttonClicked();
    void buttonSignal();

private:
    QObject *componentObject;
};

void tst_commondialog::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_commondialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_commondialog::titleText()
{
    QVERIFY(componentObject->property("titleText").isValid());
    QCOMPARE(componentObject->property("titleText").toString(), QString("title"));
}

void tst_commondialog::titleIcon()
{
    QVERIFY(componentObject->property("titleIcon").isValid());
    QCOMPARE(componentObject->property("titleIcon").toString(), QString("icon"));
}

void tst_commondialog::buttonTexts()
{
    QVERIFY(componentObject->property("buttonTexts").isValid());
    QStringList list = componentObject->property("buttonTexts").value<QStringList>();
    QCOMPARE(list[0], QString("OK"));
    QCOMPARE(list[1], QString("Cancel"));
}

void tst_commondialog::buttonClicked()
{
    QVERIFY(componentObject->metaObject()->indexOfSignal("buttonClicked(int)") != -1);
}

void tst_commondialog::buttonSignal()
{
    //find the buttons to simulate a click
    QDeclarativeItem *dialogButtons = componentObject->findChild<QDeclarativeItem *>("buttonRow");
    QVERIFY(dialogButtons);
    QObject *button = dialogButtons->children().at(0);

    QSignalSpy buttonClickedSpy(componentObject, SIGNAL(buttonClicked(int)));
    QVERIFY(buttonClickedSpy.isValid());
    QVERIFY(componentObject->property("status").isValid());

    QTRY_COMPARE(componentObject->property("status").toInt(), 3);
    QVERIFY(QMetaObject::invokeMethod(button, "clicked", Qt::DirectConnection));
    QCOMPARE(buttonClickedSpy.count(), 0);

    QVERIFY(QMetaObject::invokeMethod(componentObject, "open"));
    QTRY_COMPARE(componentObject->property("status").toInt(), 1);
    QVERIFY(QMetaObject::invokeMethod(button, "clicked", Qt::DirectConnection));
    QCOMPARE(buttonClickedSpy.count(), 1);
}

QTEST_MAIN(tst_commondialog)

#include "tst_commondialog.moc"
