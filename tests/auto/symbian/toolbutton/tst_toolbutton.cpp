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

#include <QTest>
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDebug>
#include "tst_quickcomponentstest.h"

class tst_toolbutton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void existingProperties();
    void initialProperties();
    void propertiesFunctionality();
    void implicitSize();

private:
    QObject *componentObject;
};


void tst_toolbutton::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_toolbutton.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_toolbutton::existingProperties()
{
    QGraphicsObject* toolButton = componentObject->findChild<QGraphicsObject*>("toolButton");
    QVERIFY(toolButton);
    QVERIFY(toolButton->property("checkable").isValid());
    QVERIFY(toolButton->property("checked").isValid());
    QVERIFY(toolButton->property("enabled").isValid());
    QVERIFY(toolButton->property("text").isValid());
    QVERIFY(toolButton->property("iconSource").isValid());
    QVERIFY(toolButton->property("flat").isValid());
    QVERIFY(toolButton->property("pressed").isValid());
}

void tst_toolbutton::initialProperties()
{
    QGraphicsObject* toolButton = componentObject->findChild<QGraphicsObject*>("toolButton");
    QVERIFY(toolButton);

    QCOMPARE(toolButton->property("checkable").toBool(), false);
    QCOMPARE(toolButton->property("checked").toBool(), false);
    QCOMPARE(toolButton->property("enabled").toBool(), true);
    QVERIFY(toolButton->property("text").toString().isNull());
    QVERIFY(toolButton->property("iconSource").toString().isNull());
    QCOMPARE(toolButton->property("flat").toBool(), false);
    QCOMPARE(toolButton->property("pressed").toBool(), false);
}

void tst_toolbutton::propertiesFunctionality()
{
    QGraphicsObject* toolButton = componentObject->findChild<QGraphicsObject*>("toolButton");
    QVERIFY(toolButton);
    const QMetaObject *metaInfo = toolButton->metaObject();
    const int metaPropertyCount = metaInfo->propertyCount();
    int propertyCount = 0;

    for (int i = 0; i < metaPropertyCount; i++) {
        QMetaProperty property = metaInfo->property(i);

        if (property.name() == QString("checkable")) {
            propertyCount++;
            property.write(toolButton,true);
            QCOMPARE(property.read(toolButton).toBool(), true);
        }

        if (property.name() == QString("checked")) {
            propertyCount++;
            property.write(toolButton,true);
            QCOMPARE(property.read(toolButton).toBool(), true);
        }

        // Enabled is found twice
        if (property.name() == QString("enabled")) {
            propertyCount++;
            property.write(toolButton,false);
            QCOMPARE(property.read(toolButton).toBool(), false);
        }

        if (property.name() == QString("text")) {
            propertyCount++;
            property.write(toolButton,"toolbutton_text");
            QCOMPARE(property.read(toolButton).toString() , QString("toolbutton_text"));
        }

        if (property.name() == QString("iconSource")) {
            propertyCount++;
            property.write(toolButton,"icon_name");
            QCOMPARE(property.read(toolButton).toUrl() , QUrl("icon_name"));
        }

        if (property.name() == QString("flat")) {
            propertyCount++;
            property.write(toolButton,true);
            QCOMPARE(property.read(toolButton).toBool(), true);
        }

        if (property.name() == QString("pressed")) {
            propertyCount++;
            // read-only
        }
    }

    QCOMPARE(propertyCount, 8);
}

void tst_toolbutton::implicitSize()
{
    QDeclarativeItem *toolButton = componentObject->findChild<QDeclarativeItem*>("toolButton");
    QVERIFY(toolButton);

    // Text
    toolButton->setProperty("text", "Text");
    toolButton->setProperty("iconVisible", false);
    toolButton->setProperty("iconFrameVisible", false);
    const qreal textImplicitWidth = toolButton->implicitWidth();
    const qreal textImplicitHeight = toolButton->implicitHeight();
    QCOMPARE(textImplicitWidth > 5, true); // needs to be something for the tool
    QCOMPARE(textImplicitHeight > 5, true); // needs to be something for the tool

    // Icon
    toolButton->setProperty("text", "");
    toolButton->setProperty("iconVisible", true);
    toolButton->setProperty("iconFrameVisible", true);
    const qreal iconImplicitWidth = toolButton->implicitWidth();
    const qreal iconImplicitHeight = toolButton->implicitHeight();
    QCOMPARE(iconImplicitWidth > 5, true);
    QCOMPARE(iconImplicitHeight > 5, true);
    QCOMPARE(iconImplicitWidth < textImplicitWidth, true);
    QCOMPARE(iconImplicitHeight, textImplicitHeight);
}

QTEST_MAIN(tst_toolbutton)
#include "tst_toolbutton.moc"
