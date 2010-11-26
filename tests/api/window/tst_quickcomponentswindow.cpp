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

class tst_quickcomponentswindow : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentswindow();

private slots:
    void pages();
    void title();
    void minimized();
    void activePage();
    void orientation();
    void nextPage();
    void previousPage();
    void quit();

private:
    QStringList standard;
    QString qmlSource;

    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};


tst_quickcomponentswindow::tst_quickcomponentswindow()
{
    QDeclarativeWindow *window = new QDeclarativeWindow();
    engine = window->engine();
    QDeclarativeComponent *component = new QDeclarativeComponent(engine);

    QFile file("tst_quickcomponentswindow.qml");
    QVERIFY(file.open(QFile::ReadOnly));
    component->setData( file.readAll(), QUrl() );
    
    componentObject = component->create();
    QVERIFY(componentObject != 0);
}

void tst_quickcomponentswindow::pages()
{
    // check property is pre-defined and set initial value
    QVERIFY( componentObject->property("pages.children").toInt()>0 );
}

void tst_quickcomponentswindow::title()
{
    // check property is pre-defined and set initial value
    QVERIFY( componentObject->setProperty("title","FooBar" ) );
    QCOMPARE( componentObject->property("title").toString() ,QString("FooBar") );
}

void tst_quickcomponentswindow::minimized()
{
    QVERIFY( componentObject->setProperty("minimized",true ) );
    // TODO: Need a way to verify the window is minimized
    QCOMPARE( componentObject->property("minimized").toBool(), true );
}

void tst_quickcomponentswindow::activePage()
{
    QVERIFY( componentObject->metaObject()->indexOfProperty("activePage") != -1 );
}

void tst_quickcomponentswindow::orientation()
{
    QVERIFY( componentObject->setProperty("orientation", Qt::Horizontal ) );
    QCOMPARE( componentObject->property("orientation").toInt() , (int)Qt::Horizontal );
}

void tst_quickcomponentswindow::nextPage()
{
    QVERIFY( componentObject->metaObject()->indexOfMethod("nextPage") != -1 );
}

void tst_quickcomponentswindow::previousPage()
{
    QVERIFY( componentObject->metaObject()->indexOfMethod("previousPage") != -1 );
}

void tst_quickcomponentswindow::quit()
{
    QVERIFY( componentObject->metaObject()->indexOfMethod("quit") != -1 );
}

QTEST_MAIN(tst_quickcomponentswindow)
#include "tst_quickcomponentswindow.moc"
