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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativeview.h>
#include <QtDeclarative/qdeclarativeitem.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentstoolbar : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void platformInverted();
    void setToolsA();
    void setToolsB();
    void setToolsToNull();
    void toolbarlayout();

private:
    static bool isVisible(QObject*);
    QObject *componentObject;
};

void tst_quickcomponentstoolbar::initTestCase()
{
    QString errors;

    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstoolbar.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentstoolbar::platformInverted()
{
    QGraphicsObject* toolBar = componentObject->findChild<QGraphicsObject*>("toolbar");
    QVERIFY(toolBar);
    QVERIFY(toolBar->property("platformInverted").isValid());
    QCOMPARE(toolBar->property("platformInverted").toBool(), false);
    toolBar->setProperty("platformInverted", QVariant(true));
    QCOMPARE(toolBar->property("platformInverted").toBool(), true);
}

void tst_quickcomponentstoolbar::setToolsA()
{
    QVariant retPage;

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("undefined"));
    QMetaObject::invokeMethod(componentObject, "setToolsA", Q_RETURN_ARG(QVariant, retPage));

    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("toolsA"));
    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsA")));
    QVERIFY(retPage.isValid());

    QObject *layout = qvariant_cast<QObject*>(retPage);
    QCOMPARE(layout->objectName(), QString("toolsA"));
    QVERIFY(isVisible(layout));

    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsB")));
    QVERIFY(retPage.isValid());

    layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(!isVisible(layout));
}

void tst_quickcomponentstoolbar::setToolsB()
{
    QVariant retPage;
    QMetaObject::invokeMethod(componentObject, "setToolsB", Q_RETURN_ARG(QVariant, retPage));
    QCOMPARE(componentObject->property("currentToolsName").toString(), QString("toolsB"));
    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsA")));

    QVERIFY(retPage.isValid());
    QObject *layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(!isVisible(layout ));

    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsB")));
    QVERIFY(retPage.isValid());
    layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(isVisible(layout));
}

void tst_quickcomponentstoolbar::setToolsToNull()
{
    QVariant toolsNull;
    QMetaObject::invokeMethod(componentObject, "setToolsToNull", Q_RETURN_ARG(QVariant, toolsNull));
    QVERIFY(toolsNull.toBool());
    QCOMPARE(componentObject->property("currentToolsName").toString(),QString("undefined"));
}

void tst_quickcomponentstoolbar::toolbarlayout()
{
    QVariant retPage;
    QMetaObject::invokeMethod(componentObject, "setToolsB");
    QMetaObject::invokeMethod(componentObject, "getLayout", Q_RETURN_ARG(QVariant, retPage), Q_ARG(QVariant, QVariant("commonToolsB")));

    QVERIFY(retPage.isValid());
    QObject *layout = qvariant_cast<QObject*>(retPage);
    QVERIFY(isVisible(layout ));

    layout->setProperty("backButton", QVariant(false));
    QVERIFY(!layout->property("backButton").toBool());

    layout->setProperty("backButton", QVariant(true));
    QVERIFY(layout->property("backButton").toBool());
}

bool tst_quickcomponentstoolbar::isVisible(QObject* obj)
{
    if (!obj)
        return false;
    int idx = obj->metaObject()->indexOfProperty("visible");
    if (idx < 0)
        return false;
    QMetaProperty prop = obj->metaObject()->property(idx);
    return prop.read(obj).toBool();
}

QTEST_MAIN(tst_quickcomponentstoolbar)

#include "tst_quickcomponentstoolbar.moc"
