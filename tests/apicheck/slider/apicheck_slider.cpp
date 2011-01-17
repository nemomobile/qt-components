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

#include "apicheckbase.h"

class ApiCheckSlider : public ApiCheckBase
{
    Q_OBJECT
private slots:
    void initTestCase();
    void stepSize();
    void minimumValue();
    void maximumValue();
    void value();
    void orientation(); //int
    void pressed(); //bool
    void updateValueWhileDragging(); //bool
};


void ApiCheckSlider::initTestCase()
{
    init("ApiCheckSlider.qml");
}

void ApiCheckSlider::stepSize()
{
    validateProperty("stepSize", QVariant::Double, 1.0);
}

void ApiCheckSlider::minimumValue()
{
    validateProperty("minimumValue", QVariant::Double, 0.0);
}

void ApiCheckSlider::maximumValue()
{
    validateProperty("maximumValue", QVariant::Double, 100.0);
}

void ApiCheckSlider::value()
{
    validateProperty("value", QVariant::Double, 0.0);
}

void ApiCheckSlider::orientation()
{
    validateProperty("orientation", QVariant::Int, Qt::Horizontal);
}

void ApiCheckSlider::pressed()
{
    validateProperty("pressed", QVariant::Bool);
}

void ApiCheckSlider::updateValueWhileDragging()
{
    validateProperty("updateValueWhileDragging", QVariant::Bool, true);
}

QTEST_MAIN(ApiCheckSlider)

#include "apicheck_slider.moc"
