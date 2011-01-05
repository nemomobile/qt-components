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
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <qdeclarativewindow.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentstoolbar : public QObject

{
    Q_OBJECT

private slots:
    void initTestCase();
    void setToolsA();
    void setToolsB();

private:
    QObject *componentObject;
};

void tst_quickcomponentstoolbar::initTestCase()
{
    QString errors;

    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstoolbar.qml", &errors);

    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentstoolbar::setToolsA()
{
    QVariant retPage;

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("undefined"));
    QMetaObject::invokeMethod(componentObject, "setToolsA", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("toolsA"));
}

void tst_quickcomponentstoolbar::setToolsB()
{
    QVariant retPage;

    QMetaObject::invokeMethod(componentObject, "setToolsB", Qt::DirectConnection, Q_RETURN_ARG(QVariant, retPage));

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("toolsB"));
}

QTEST_MAIN(tst_quickcomponentstoolbar)

#include "tst_quickcomponentstoolbar.moc"
