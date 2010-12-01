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

class tst_quickcomponentschoicelist : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentschoicelist();

private slots:
    void model();
    void delegate();
    void currentIndex();

private:
    QStringList standard;
    QString qmlSource;

    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};


tst_quickcomponentschoicelist::tst_quickcomponentschoicelist()
{
    QDeclarativeWindow *window = new QDeclarativeWindow();
    engine = window->engine();
    QDeclarativeComponent *component = new QDeclarativeComponent(engine);

    QFile file("tst_quickcomponentschoicelist.qml");
    QVERIFY(file.open(QFile::ReadOnly));
    component->setData( file.readAll(), QUrl() );
    
    componentObject = component->create();
    QVERIFY(componentObject != 0);
}

void tst_quickcomponentschoicelist::model()
{
    // check presence of property and set initial value
    QVERIFY( componentObject->setProperty( "model", 5 ) );
}

void tst_quickcomponentschoicelist::delegate()
{
    // check presence of property and set initial value
    QDeclarativeComponent *delegate = new QDeclarativeComponent(engine);
    delegate->setData( "import QtQuick 1.0\n"
                       "Rectangle {\n"
                          "height: 30; width: parent.width;\n"
                          "Text {\n"
                             "text: \"I'm item number'\" + index\n"
                          "}\n"
                       "}\n", QUrl() );
    QVERIFY( componentObject->setProperty( "delegate", QVariant::fromValue(delegate->create()) ) );
}

void tst_quickcomponentschoicelist::currentIndex()
{
    QCOMPARE( componentObject->property("currentIndex").toInt(), 0 );
}

QTEST_MAIN(tst_quickcomponentschoicelist)
#include "tst_quickcomponentschoicelist.moc"
