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

class tst_quickcomponentspage : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentspage();

private slots:
    void title();
    void actions();
    void aboutToEnter();
    void entered();
    void aboutToExit();
    void exited();
    void blocked();
    void close();

private:
    QStringList standard;
    QString qmlSource;

    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};


tst_quickcomponentspage::tst_quickcomponentspage()
{
    QDeclarativeWindow *window = new QDeclarativeWindow();
    engine = window->engine();
    QDeclarativeComponent *component = new QDeclarativeComponent(engine);

    QFile file("tst_quickcomponentspage.qml");
    QVERIFY(file.open(QFile::ReadOnly));
    component->setData( file.readAll(), QUrl() );
    
    componentObject = component->create();
    QVERIFY(componentObject != 0);
}

void tst_quickcomponentspage::title()
{
    // check property is pre-defined and set initial value
    QVERIFY( componentObject->setProperty("title","FooBar" ) );
    QCOMPARE( componentObject->property("title").toString() , QString("FooBar") );
}

void tst_quickcomponentspage::actions()
{
}

void tst_quickcomponentspage::aboutToEnter()
{
    QSignalSpy spy(componentObject, SIGNAL(aboutToEnter()));
    QMetaObject::invokeMethod(componentObject,"aboutToEnter",
                              Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

void tst_quickcomponentspage::entered()
{
    QSignalSpy spy(componentObject, SIGNAL(entered()));
    QMetaObject::invokeMethod(componentObject,"entered",
                              Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

void tst_quickcomponentspage::aboutToExit()
{
    QSignalSpy spy(componentObject, SIGNAL(aboutToExit()));
    QMetaObject::invokeMethod(componentObject,"aboutToExit",
                              Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

void tst_quickcomponentspage::exited()
{
    QVERIFY( componentObject->metaObject()->indexOfMethod("exited") != -1 );

    QSignalSpy spy(componentObject, SIGNAL(exited()));
    QMetaObject::invokeMethod(componentObject,"exited",
                              Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}

void tst_quickcomponentspage::blocked()
{
    QSignalSpy spy(componentObject, SIGNAL(blocked()));
    QMetaObject::invokeMethod(componentObject,"blocked",
                              Qt::DirectConnection);
    QCOMPARE(spy.count(),1);
    QVERIFY(spy.isValid());
}


void tst_quickcomponentspage::close()
{
    QVERIFY( componentObject->metaObject()->indexOfMethod("close") != -1 );
}

QTEST_MAIN(tst_quickcomponentspage)
#include "tst_quickcomponentspage.moc"
