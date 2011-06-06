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
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativeview.h>

#include "tst_quickcomponentstest.h"

class tst_ratingindicator : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void maximumValue();
    void ratingValue();
    void count();

private:
    QObject *componentObject;
};


void tst_ratingindicator::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_ratingindicator.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_ratingindicator::maximumValue()
{
    componentObject->setProperty("maximumValue", 4.0);
    QCOMPARE(componentObject->property("maximumValue").toReal(), 4.0);
}

void tst_ratingindicator::ratingValue()
{
    componentObject->setProperty("maximumValue", 5.0);
    QCOMPARE(componentObject->property("maximumValue").toReal(), 5.0);
    componentObject->setProperty("ratingValue", 4.0);
    QCOMPARE(componentObject->property("ratingValue").toReal(), 4.0);

    // check that ratingValue cannot be set to value above maximumValue
    componentObject->setProperty("ratingValue", 6.0);
    QCOMPARE(componentObject->property("ratingValue").toReal(), 5.0);
}

void tst_ratingindicator::count()
{
    // check that default count value is -1
    QCOMPARE(componentObject->property("count").toInt(), -1);

    componentObject->setProperty("count", 2);
    QCOMPARE(componentObject->property("count").toInt(), 2);
}

QTEST_MAIN(tst_ratingindicator)

#include "tst_ratingindicator.moc"
