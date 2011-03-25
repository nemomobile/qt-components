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
#include <QGraphicsObject>
#include "tst_quickcomponentstest.h"

class tst_slider : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void defaultPropertyValues();
    void properties();

private:
    QObject *componentObject;
};

void tst_slider::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_slider.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_slider::cleanupTestCase()
{
}

void tst_slider::init()
{
}

void tst_slider::cleanup()
{
}

void tst_slider::defaultPropertyValues()
{
    QVERIFY(componentObject->property("inverted").isValid());
    QCOMPARE(componentObject->property("inverted").toBool(), false);
}

void tst_slider::properties()
{
    // check presence of the property "inverted" and set a value
    QVERIFY(componentObject->setProperty("inverted", false));
    QCOMPARE(componentObject->property("inverted").toBool(), false);
    // set the property and compare
    QVERIFY(componentObject->setProperty("inverted", true));
    QCOMPARE(componentObject->property("inverted").toBool(), true);
}

QTEST_MAIN(tst_slider)
#include "tst_slider.moc"
