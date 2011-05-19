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

#include <qtest.h>
#include "tst_quickcomponentstest.h"

class tst_quickcomponentsnetworkindicator : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void validateProperties();

private:
    QObject *m_componentObject;
};

void tst_quickcomponentsnetworkindicator::initTestCase()
{
    QString errors;
    m_componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsnetworkindicator.qml", &errors);
    QVERIFY2(m_componentObject, qPrintable(errors));
}

void tst_quickcomponentsnetworkindicator::validateProperties()
{
    QVariant mode = m_componentObject->property("color");
    QVERIFY(mode.isValid());
    QVERIFY(mode.canConvert(QVariant::Color));
}


QTEST_MAIN(tst_quickcomponentsnetworkindicator)

#include "tst_quickcomponentsnetworkindicator.moc"
