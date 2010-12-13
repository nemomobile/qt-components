/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QDeclarativeView>
#include <qdeclarativewindow.h>

class tst_quickcomponentsbutton : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentsbutton();

private slots:
    void checked();
    void checkable();
    void clicked();
    void text();
    void iconSource();

private:
    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};

tst_quickcomponentsbutton::tst_quickcomponentsbutton()
{
    QDeclarativeWindow *window = new QDeclarativeWindow;
    QDeclarativeComponent *component = new QDeclarativeComponent(window->engine());

    QFile file("tst_quickcomponentsbutton.qml");
    QVERIFY(file.open(QFile::ReadOnly));
    component->setData( file.readAll(), QUrl() );
    
    QVERIFY( (componentObject = component->create()) != 0);
}

void tst_quickcomponentsbutton::checkable()
{
    // make sure properties are present and set initial values
    QVERIFY( componentObject->setProperty("checked", false) );
    QVERIFY( componentObject->setProperty("checkable", false) );

    // test both properties have right initial value
    QCOMPARE( componentObject->property("checkable").toBool(), false );
    QCOMPARE( componentObject->property("checked").toBool(), false );

    // try to set the property while checkable is false
    QVERIFY( componentObject->setProperty("checked",true) );
    QEXPECT_FAIL("", "QML Doesn't have readonly properties", Continue);
    QCOMPARE( componentObject->property("checked").toBool(),false );

    // allow checked to change again, and verify it changes
    QVERIFY( componentObject->setProperty("checkable",true) );
    QVERIFY( componentObject->setProperty("checked",true) );
    QCOMPARE( componentObject->property("checked").toBool(),true );
}

void tst_quickcomponentsbutton::checked()
{
    // make sure properties are present and set initial values
    QVERIFY( componentObject->setProperty("checked",false) );
    QVERIFY( componentObject->setProperty("checkable",true) );

    // test switching the checked property
    QCOMPARE( componentObject->property("checked").toBool(),false );
    QVERIFY( componentObject->setProperty("checked",true) );
    QCOMPARE( componentObject->property("checked").toBool(), true );
}
void tst_quickcomponentsbutton::text()
{
    QCOMPARE(componentObject->property("text").toString(),QString("Testing Button"));
    componentObject->setProperty("text","Button Tested!");
    QCOMPARE(componentObject->property("text").toString(),QString("Button Tested!"));
}

void tst_quickcomponentsbutton::iconSource()
{
    QCOMPARE(componentObject->property("iconSource").toString(),QString("http://qt.nokia.com/logo.png"));
}

void tst_quickcomponentsbutton::clicked()
{
    QSignalSpy spy(componentObject, SIGNAL(clicked()));
    QMetaObject::invokeMethod(componentObject,"clicked",Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

QTEST_MAIN(tst_quickcomponentsbutton)

#include "tst_quickcomponentsbutton.moc"
