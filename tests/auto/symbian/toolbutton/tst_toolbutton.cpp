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
    QVERIFY(toolButton->property("platformInverted").isValid());
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
    QCOMPARE(toolButton->property("platformInverted").toBool(), false);
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

        if (property.name() == QString("platformInverted")) {
            propertyCount++;
            property.write(toolButton,true);
            QCOMPARE(property.read(toolButton).toBool(), true);
        }
    }

    QCOMPARE(propertyCount, 9);
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

    QGraphicsObject* rootItem = qobject_cast<QGraphicsObject*>(componentObject);
    QVERIFY(rootItem);
    if(rootItem->property("portrait").toBool()) {
        QCOMPARE(iconImplicitHeight > textImplicitHeight, true);
    }
    else {
        QCOMPARE(iconImplicitHeight, textImplicitHeight);
    }
}

QTEST_MAIN(tst_toolbutton)
#include "tst_toolbutton.moc"
