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

#include <QObject>
#include <QObjectList>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include <QDeclarativeItem>

#include <qdeclarativecontext.h>
#include <qdeclarativecomponent.h>
#include <qdeclarativeview.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsselectiondialog : public QObject

{
   Q_OBJECT

private slots:
    void initTestCase();
    void initSelectionDialog();
    void selectedIndex();

private:
    QObject *componentObject;
    int mAnimationTime;

};


void tst_quickcomponentsselectiondialog::initTestCase()
{
    mAnimationTime = 1000; //ms

    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsselectiondialog.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));

    // check, that dialog has a parent
    QVERIFY( componentObject->children().count() > 1 );
    componentObject = componentObject->findChild<QObject *>("testTarget");
    QVERIFY(componentObject);

}


void tst_quickcomponentsselectiondialog::initSelectionDialog()
{
    // make sure properties are present and set initial values
    QVERIFY( componentObject->setProperty("titleText", "TitleText"));
    QVERIFY( componentObject->setProperty("selectedIndex", 1));

    QVariant modelVariant = componentObject->property("model");
    QVERIFY(modelVariant.isValid());

}


void tst_quickcomponentsselectiondialog::selectedIndex()
{
    QCOMPARE( componentObject->property("selectedIndex").toInt(), 1 );

    // Change selected index to -1
    QVERIFY( componentObject->setProperty("selectedIndex", -1) );

    // test the property has the right value
    QCOMPARE( componentObject->property("selectedIndex").toInt(), -1 );
}



QTEST_MAIN(tst_quickcomponentsselectiondialog)

#include "tst_quickcomponentsselectiondialog.moc"
