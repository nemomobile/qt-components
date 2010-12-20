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

class tst_quickcomponentsscrolldecorator : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void flickable();

private:
    QStringList standard;
    QString qmlSource;

    QDeclarativeComponent *component;
    QObject *componentObject;
    QDeclarativeEngine *engine;
};


void tst_quickcomponentsscrolldecorator::initTestCase()
{
    QDeclarativeWindow *window = new QDeclarativeWindow();
    window->engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");
    QDeclarativeComponent *component = new QDeclarativeComponent(window->engine());

    QFile file("tst_quickcomponentsscrolldecorator.qml");
    if( file.open(QFile::ReadOnly) )
        component->setData( file.readAll(), QUrl() );

    componentObject = component->create();
    QVERIFY(componentObject);
}

void tst_quickcomponentsscrolldecorator::flickable()
{
    // check presence of a real flickable element
    QCOMPARE( componentObject->property("flickable").typeName(), "QDeclarativeFlickable*");
}

QTEST_MAIN(tst_quickcomponentsscrolldecorator)

#include "tst_quickcomponentsscrolldecorator.moc"
