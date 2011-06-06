/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativeview.h>
#include <QDeclarativeItem>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsbuttoncolumn : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void exclusive();
    void checkedButton();

private:
    QObject *componentObject;
};

void tst_quickcomponentsbuttoncolumn::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsbuttoncolumn.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsbuttoncolumn::exclusive()
{
    // make sure property is present and set initial value
    QVERIFY(componentObject->setProperty("exclusive", false));

    // test property has right initial value
    QCOMPARE(componentObject->property("exclusive").toBool(), false);

    // first we set the checkable property of all Buttons that are
    // children of ButtonRow to false.
    QObject *child;
    const QObjectList children = componentObject->children();
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("child_")) {
            QVERIFY2(child->property("checked").isValid(), "Error, ButtonColumn's child must have checked property");
            if (child->property("checkable").isValid()) {
                child->setProperty("checkable", false);
                QVERIFY2(child->property("checkable").toBool() == false, QString("Error, could not set checkable property of %1").arg(child->metaObject()->className()).toAscii().data());
            }
        }
    }

    // change value to true and test again
    QVERIFY(componentObject->setProperty("exclusive",true) );
    QCOMPARE(componentObject->property("exclusive").toBool(), true);

    // According to the Common API: "when true, forces all its children to be checkable"
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("child_")) {
            if (child->property("checkable").isValid())
                QVERIFY2(child->property("checkable").toBool() == true, QString("Error, checkable property was not set to true in %1").arg(child->metaObject()->className()).toAscii().data());
        }
    }
}

void tst_quickcomponentsbuttoncolumn::checkedButton()
{
    // get ButtonColumn first Button child
    QVariant variant;
    QObject *child;
    QDeclarativeItem  *tmp;
    const QObjectList children = componentObject->children();
    for (int i = 0; i < children.size(); ++i) {
        child = children.at(i);
        if (child->objectName().contains("child_")) {
            QVERIFY2(QMetaObject::invokeMethod(child, "clicked"), "ButtonColumn's child, does not have clicked() signal");
            variant = componentObject->property("checkedButton");
            QVERIFY(variant.isValid());
            tmp = variant.value<QDeclarativeItem*>();
            QVERIFY2(child->objectName() == tmp->objectName(), "Error, the returned object is not the one that was previously set");
        }
    }
}

QTEST_MAIN(tst_quickcomponentsbuttoncolumn)

#include "tst_quickcomponentsbuttoncolumn.moc"
