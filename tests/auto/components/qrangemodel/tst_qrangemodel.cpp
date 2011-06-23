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
#include "models/qrangemodel.h"

typedef QList<qreal> RealList;

class tst_QRangeModel : public QObject
{
    Q_OBJECT

public:
    tst_QRangeModel();
    virtual ~tst_QRangeModel();

private slots:
    void valueAndPosition_data();
    void valueAndPosition();

    void sameRange();
    void setValueAndSetRangeOrder();
    void setRange();
    void setPositionRange();
    void valueMustNotChange();
    void outOfRangeValues();
    void outOfRangePositions();
    void toMaximum();
    void toMinimum();
    void invertedTest();

    void valueForPosition_data();
    void valueForPosition();
    void positionForValue_data();
    void positionForValue();

    void stepSize();
    void invertedStepSize();

    void settingOwnPositionAndValue();
};

tst_QRangeModel::tst_QRangeModel()
{
}

tst_QRangeModel::~tst_QRangeModel()
{
}

void tst_QRangeModel::toMaximum()
{
    QRangeModel m;

    //### NEW RANGE
    m.setRange(0.0, 100.0);
    m.setPositionRange(0.0, 100.0);

    QList<QVariant> args;

    //valueChanged and positionChanged signals must be checked
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    m.toMaximum();
    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    args = valueChangedSpy.at(0);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(positionChangedSpy.count(), 1);
    args = positionChangedSpy.at(0);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(50.0, 150.0);

    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 50.0);
    QCOMPARE(positionChangedSpy.count(), 2);
    args = positionChangedSpy.at(1);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMaximum();
    QCOMPARE(m.value(), 150.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    args = valueChangedSpy.at(1);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 3);
    args = positionChangedSpy.at(2);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(0.0, 100.0);

    QCOMPARE(m.value(), 100.0);
    QCOMPARE(valueChangedSpy.count(), 3);
    args = valueChangedSpy.at(2);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 3);

    //### NEW RANGE
    m.setRange(-100.0, 300.0);

    QCOMPARE(m.value(), 150.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    QCOMPARE(m.position(), 62.5);
    QCOMPARE(positionChangedSpy.count(), 4);
    args = positionChangedSpy.at(3);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMaximum();
    QCOMPARE(m.value(), 300.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    args = valueChangedSpy.at(4);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 5);

    //### NEW RANGE
    m.setRange(-200.0, -100.0);

    QCOMPARE(m.value(), -100.0);
    QCOMPARE(valueChangedSpy.count(), 6);
    args = valueChangedSpy.at(5);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 5);

    m.setValue(-150.0);
    m.toMaximum();
    QCOMPARE(m.value(), -100.0);
    QCOMPARE(valueChangedSpy.count(), 8);
    args = valueChangedSpy.at(7);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 7);
    args = positionChangedSpy.at(6);
    QCOMPARE(m.position(), args.at(0).toReal());
}

void tst_QRangeModel::toMinimum()
{
    QRangeModel m;
    //### NEW RANGE
    m.setRange(50.0, 100.0);
    m.setPositionRange(0.0, 100.0);

    //valueChanged and positionChanged signals must be checked
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));
    QList<QVariant> args;

    QCOMPARE(m.value(), 50.0);
    QCOMPARE(m.position(), 0.0);

    m.toMinimum();
    QCOMPARE(m.value(), 50.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 0);

    //### NEW RANGE
    m.setRange(0.0, 200.0);

    QCOMPARE(m.value(), 50.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(m.position(), 25.0);
    QCOMPARE(positionChangedSpy.count(), 1);
    args = positionChangedSpy.at(0);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMinimum();
    QCOMPARE(m.value(), 0.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    args = valueChangedSpy.at(0);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(positionChangedSpy.count(), 2);
    args = positionChangedSpy.at(1);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(100.0, 150.0);

    QCOMPARE(m.value(), 100.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    args = valueChangedSpy.at(1);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 2);

    //### NEW RANGE
    m.setRange(-100.0, 300.0);

    QCOMPARE(m.value(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 3);
    QCOMPARE(m.position(), 25.0);
    QCOMPARE(positionChangedSpy.count(), 3);
    args = positionChangedSpy.at(2);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMinimum();
    QCOMPARE(m.value(), -100.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    args = valueChangedSpy.at(3);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 4);
    args = positionChangedSpy.at(3);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(-200.0, -100.0);

    QCOMPARE(m.value(), -100.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 5);
    args = positionChangedSpy.at(4);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMinimum();
    QCOMPARE(m.value(), -200.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    args = valueChangedSpy.at(4);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 6);
    args = positionChangedSpy.at(5);
    QCOMPARE(m.position(), args.at(0).toReal());
}

void tst_QRangeModel::invertedTest()
{
    QRangeModel m;
    //setting ranges
    m.setRange(0.0, 100.0);
    m.setPositionRange(0.0, 100.0);
    //setting inverted property to TRUE

    QSignalSpy invertedSpy(&m, SIGNAL(invertedChanged(bool)));
    QList<QVariant> args;

    m.setInverted(true);
    QCOMPARE(m.inverted(), true);
    QCOMPARE(invertedSpy.count(), 1);
    QCOMPARE(invertedSpy.at(0).at(0).toBool(), true);

    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    m.setValue(10.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    args = valueChangedSpy.at(0);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 90.0);
    QCOMPARE(positionChangedSpy.count(), 1);
    args = positionChangedSpy.at(0);
    QCOMPARE(m.position(), args.at(0).toReal());

    //reducing range. value must stay constant, but position must be updated
    //### NEW RANGE
    m.setRange(0.0, 50.0);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 80.0);
    QCOMPARE(positionChangedSpy.count(), 2);
    args = positionChangedSpy.at(1);
    QCOMPARE(m.position(), args.at(0).toReal());

    //setting range smaller, in order to make current value to be smaller than the minimum
    //since value is smaller than the minimum, it must be reseted to minimum
    //### NEW RANGE
    m.setRange(50.0, 100.0);

    QCOMPARE(m.value(), 50.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    args = valueChangedSpy.at(1);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 3);
    args = positionChangedSpy.at(2);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setValue(50.0);
    QCOMPARE(m.value(), 50.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 3);

    //### NEW RANGE
    m.setRange(-100.0, 0.0);

    QCOMPARE(m.value(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 3);
    args = valueChangedSpy.at(2);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 4);
    args = positionChangedSpy.at(3);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setValue(-75.0);
    QCOMPARE(m.value(), -75.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    args = valueChangedSpy.at(3);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 75.0);
    QCOMPARE(positionChangedSpy.count(), 5);
    args = positionChangedSpy.at(4);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(-100.0, -50.0);

    QCOMPARE(m.value(), -75.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    QCOMPARE(m.position(), 50.0);
    QCOMPARE(positionChangedSpy.count(), 6);
    args = positionChangedSpy.at(5);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setValue(-90.0);
    QCOMPARE(m.value(), -90.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    args = valueChangedSpy.at(4);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 80.0);
    QCOMPARE(positionChangedSpy.count(), 7);
    args = positionChangedSpy.at(6);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setInverted(false);
    QCOMPARE(m.inverted(), false);
    QCOMPARE(invertedSpy.count(), 2);
    QCOMPARE(invertedSpy.at(1).at(0).toBool(), false);

    QCOMPARE(m.value(), -90.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(positionChangedSpy.count(), 8);
    args = positionChangedSpy.at(7);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setRange(0.0, 100.0);
    m.setPositionRange(0.0, 100.0);
    m.setInverted(true);
    m.setStepSize(30.0);
    m.setValue(91.0);
    QCOMPARE(m.value(), 90.0);
    QCOMPARE(m.position(), 10.0);
}

void tst_QRangeModel::valueAndPosition_data()
{
    QTest::addColumn<qreal>("min");
    QTest::addColumn<qreal>("max");
    QTest::addColumn<qreal>("posAtMin");
    QTest::addColumn<qreal>("posAtMax");
    // QTest::addColumn<RealList>("values");
    // QTest::addColumn<RealList>("positions");

    QTest::newRow("same range") << qreal(1.0) << qreal(2.0) << qreal(1.0) << qreal(2.0);
    QTest::newRow("same range inverted pos") << qreal(1.0) << qreal(2.0) << qreal(2.0) << qreal(1.0);
    QTest::newRow("pos: 0 100 -> value: 1 2") << qreal(1.0) << qreal(2.0) << qreal(0.0) << qreal(100.0);
    QTest::newRow("pos: 100 0 -> value: 1 2") << qreal(1.0) << qreal(2.0) << qreal(100.0) << qreal(0.0);
    QTest::newRow("pos: -200 300 -> value: 1 2") << qreal(1.0) << qreal(2.0) << qreal(-200.0) << qreal(300.0);
    QTest::newRow("pos: 300 -200 -> value: -1 2.5") << qreal(-1.0) << qreal(2.5) << qreal(300.0) << qreal(-200.0);
}

void tst_QRangeModel::valueAndPosition()
{
    QFETCH(qreal, min);
    QFETCH(qreal, max);
    QFETCH(qreal, posAtMin);
    QFETCH(qreal, posAtMax);
    // QFETCH(RealList, values);
    // QFETCH(RealList, positions);

    QRangeModel m;

    // Set the ranges
    m.setRange(min, max);
    m.setPositionRange(posAtMin, posAtMax);
    QCOMPARE(m.minimum(), min);
    QCOMPARE(m.maximum(), max);
    QCOMPARE(m.positionAtMinimum(), posAtMin);
    QCOMPARE(m.positionAtMaximum(), posAtMax);

    // Check extremes
    m.setPosition(posAtMax);
    QCOMPARE(m.position(), posAtMax);
    QCOMPARE(m.value(), max);

    m.setPosition(posAtMin);
    QCOMPARE(m.position(), posAtMin);
    QCOMPARE(m.value(), min);

    m.setValue(max);
    QCOMPARE(m.position(), posAtMax);
    QCOMPARE(m.value(), max);

    m.setValue(min);
    QCOMPARE(m.position(), posAtMin);
    QCOMPARE(m.value(), min);

    // Check "mid point", resetting to max between two tests...
    const qreal posAtMid = (posAtMax + posAtMin) / 2;
    const qreal mid = (min + max) / 2;
    m.setPosition(posAtMid);
    QCOMPARE(m.position(), posAtMid);
    QCOMPARE(m.value(), mid);

    m.setValue(max);
    QCOMPARE(m.position(), posAtMax);
    QCOMPARE(m.value(), max);

    m.setValue(mid);
    QCOMPARE(m.position(), posAtMid);
    QCOMPARE(m.value(), mid);

    // Check "1/3 between min and max values", reseting to max between two tests...
    const qreal valueAtThird = (max - min) / 3 + min;
    const qreal posAtThird = (posAtMax - posAtMin) / 3 + posAtMin;

    m.setPosition(posAtThird);
    QCOMPARE(m.position(), posAtThird);
    QCOMPARE(m.value(), valueAtThird);

    m.setValue(max);
    QCOMPARE(m.position(), posAtMax);
    QCOMPARE(m.value(), max);

    m.setValue(valueAtThird);
    QCOMPARE(m.position(), posAtThird);
    QCOMPARE(m.value(), valueAtThird);

    // TODO: use values and positions to perform checks. Assume values[i] match positions[i].
}

// ### This is a very simple instance of the previous, but keeping it
// here until we sort out the simplest bug. It's easy to tweak and
// debug from here than from the generic test.
void tst_QRangeModel::sameRange()
{
    QRangeModel m;

    m.setRange(1.0, 2.0);
    m.setPositionRange(1.0, 2.0);

    m.setPosition(2.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(m.value(), 2.0);

    m.setPosition(1.0);
    QCOMPARE(m.position(), 1.0);
    QCOMPARE(m.value(), 1.0);
}

void tst_QRangeModel::setValueAndSetRangeOrder()
{
    // QRangeModel is exported to a QML environment, so needs to play
    // nice with properties being set in a different order, possibly "caching"
    // data if necessary.

    QRangeModel m;

    const qreal newValue = m.maximum() + 1.0;
    const qreal newMax = m.maximum() + 2.0;

    QSignalSpy spy(&m, SIGNAL(valueChanged(qreal)));

    // Sets new value before the range, "public" value changes but not yet
    // what we set.
    m.setValue(newValue);
    QCOMPARE(m.value(), m.maximum());
    QCOMPARE(spy.count(), 1);

    QList<QVariant> args = spy.at(0);
    QCOMPARE(m.value(), args.at(0).toReal());

    // Now sets the range, "public" value changes again, for what we set.
    m.setMaximum(newMax);
    QCOMPARE(m.value(), newValue);
    QCOMPARE(spy.count(), 2);

    args = spy.at(1);
    QCOMPARE(m.value(), args.at(0).toReal());
}

void tst_QRangeModel::setRange()
{
    // These tests check different corner cases of setRange()
    QRangeModel m;

    // Init
    m.setRange(10, 15);
    m.setPositionRange(-1000, 1000);
    m.setValue(12);

    // Start watching for signals
    QSignalSpy maximumChangedSpy(&m, SIGNAL(maximumChanged(qreal)));
    QSignalSpy minimumChangedSpy(&m, SIGNAL(minimumChanged(qreal)));
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), -200.0);
    QCOMPARE(minimumChangedSpy.count(), 0);
    QCOMPARE(maximumChangedSpy.count(), 0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 0);

    // Standard situation, change range, value keeps constant, position changes
    m.setRange(11, 16);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), -600.0);
    QCOMPARE(minimumChangedSpy.count(), 1);
    QCOMPARE(maximumChangedSpy.count(), 1);
    QCOMPARE(minimumChangedSpy.at(0).at(0).toReal(), 11.0);
    QCOMPARE(maximumChangedSpy.at(0).at(0).toReal(), 16.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.at(0).at(0).toReal(), -600.0);

    // Test setRange() min boundary. Still, value should not change.
    m.setRange(12, 16);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), -1000.0);
    QCOMPARE(minimumChangedSpy.count(), 2);
    QCOMPARE(minimumChangedSpy.at(1).at(0).toReal(), 12.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.at(1).at(0).toReal(), -1000.0);

    // Test setRange() max boundary. Still, value should not change.
    m.setRange(8, 12);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), 1000.0);
    QCOMPARE(maximumChangedSpy.count(), 2);
    QCOMPARE(minimumChangedSpy.count(), 3);
    QCOMPARE(minimumChangedSpy.at(2).at(0).toReal(), 8.0);
    QCOMPARE(maximumChangedSpy.at(1).at(0).toReal(), 12.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionChangedSpy.at(2).at(0).toReal(), 1000.0);

    // Test setRange() out of boundary. This is a corner case.
    // 1) Internally "value" should not change. We want the model to be
    //    resilient and allow for inconsistent, transient states, often
    //    found when QML engine is initializing its properties in random order.
    // 2) OTOH, from the external point of view, it must appear consistent
    //    and clamp value() to the boundaries specified in setRange().
    m.setRange(20, 30);

    QCOMPARE(m.value(), 20.0);
    QCOMPARE(m.position(), -1000.0);
    QCOMPARE(minimumChangedSpy.count(), 4);
    QCOMPARE(maximumChangedSpy.count(), 3);
    QCOMPARE(minimumChangedSpy.at(3).at(0).toReal(), 20.0);
    QCOMPARE(maximumChangedSpy.at(2).at(0).toReal(), 30.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0).toReal(), 20.0);
    QCOMPARE(positionChangedSpy.count(), 4);
    QCOMPARE(positionChangedSpy.at(3).at(0).toReal(), -1000.0);

    // Same as above. Now overflowing value()
    m.setRange(-10, -5);

    QCOMPARE(m.value(), -5.0);
    QCOMPARE(m.position(), 1000.0);
    QCOMPARE(minimumChangedSpy.count(), 5);
    QCOMPARE(maximumChangedSpy.count(), 4);
    QCOMPARE(minimumChangedSpy.at(4).at(0).toReal(), -10.0);
    QCOMPARE(maximumChangedSpy.at(3).at(0).toReal(), -5.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(valueChangedSpy.at(1).at(0).toReal(), -5.0);
    QCOMPARE(positionChangedSpy.count(), 5);
    QCOMPARE(positionChangedSpy.at(4).at(0).toReal(), 1000.0);

    // Now we have a valid range again. Internal value must have survived.
    m.setRange(0, 20);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), 200.0);
    QCOMPARE(minimumChangedSpy.count(), 6);
    QCOMPARE(maximumChangedSpy.count(), 5);
    QCOMPARE(minimumChangedSpy.at(5).at(0).toReal(), 0.0);
    QCOMPARE(maximumChangedSpy.at(4).at(0).toReal(), 20.0);
    QCOMPARE(valueChangedSpy.count(), 3);
    QCOMPARE(valueChangedSpy.at(2).at(0).toReal(), 12.0);
    QCOMPARE(positionChangedSpy.count(), 6);
    QCOMPARE(positionChangedSpy.at(5).at(0).toReal(), 200.0);

    // Set the same range again, nothing can change
    m.setMinimum(0);
    m.setMaximum(20);
    m.setRange(0, 20);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), 200.0);
    QCOMPARE(minimumChangedSpy.count(), 6);
    QCOMPARE(maximumChangedSpy.count(), 5);
    QCOMPARE(valueChangedSpy.count(), 3);
    QCOMPARE(positionChangedSpy.count(), 6);
}

void tst_QRangeModel::setPositionRange()
{
    // Setting position range should cause position changes in most cases.
    QRangeModel m;

    // Init with 2/5 value
    m.setRange(5, 10);
    m.setPositionRange(50, 150);
    m.setValue(7);

    // Start watching for signals
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));
    QSignalSpy positionAtMaximumChangedSpy(&m, SIGNAL(positionAtMaximumChanged(qreal)));
    QSignalSpy positionAtMinimumChangedSpy(&m, SIGNAL(positionAtMinimumChanged(qreal)));

    QCOMPARE(m.value(), 7.0);
    QCOMPARE(m.position(), 90.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 0);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 0);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 0);

    // Standard situation. Change position range. Position will change
    m.setPositionRange(-100, 200);

    QCOMPARE(m.value(), 7.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.at(0).at(0).toReal(), 20.0);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 1);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 1);
    QCOMPARE(positionAtMinimumChangedSpy.at(0).at(0).toReal(), -100.0);
    QCOMPARE(positionAtMaximumChangedSpy.at(0).at(0).toReal(), 200.0);

    // Corner case situation. Position happens to remain the same.
    m.setPositionRange(-40.0, 110.0);

    QCOMPARE(m.value(), 7.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 2);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 2);
    QCOMPARE(positionAtMinimumChangedSpy.at(1).at(0).toReal(), -40.0);
    QCOMPARE(positionAtMaximumChangedSpy.at(1).at(0).toReal(), 110.0);

    // Tests with value == min
    m.setValue(5);

    QCOMPARE(m.value(), 5.0);
    QCOMPARE(m.position(), -40.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0).toReal(), 5.0);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.at(1).at(0).toReal(), -40.0);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 2);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 2);

    // Changing positionAtMaximum won't cause changes to position nor value
    m.setPositionAtMaximum(160);

    QCOMPARE(m.value(), 5.0);
    QCOMPARE(m.position(), -40.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 2);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 3);
    QCOMPARE(positionAtMaximumChangedSpy.at(2).at(0).toReal(), 160.0);

    // Tests with value == max
    m.setValue(10);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 160.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(valueChangedSpy.at(1).at(0).toReal(), 10.0);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionChangedSpy.at(2).at(0).toReal(), 160.0);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 2);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 3);

    // Changing positionAtMinimum won't cause changes to position nor value
    m.setPositionAtMinimum(260);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 160.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 3);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 3);
    QCOMPARE(positionAtMinimumChangedSpy.at(2).at(0).toReal(), 260.0);

    // Resetting same position range should not change anything
    m.setPositionRange(260, 160);
    m.setPositionAtMinimum(260);
    m.setPositionAtMaximum(160);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 160.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionAtMinimumChangedSpy.count(), 3);
    QCOMPARE(positionAtMaximumChangedSpy.count(), 3);

    m.setStepSize(3);
    m.setValue(9.0);
    QCOMPARE(m.value(), 8.0);
    QCOMPARE(m.position(), 200.0);
}

void tst_QRangeModel::valueMustNotChange()
{
    // QRangeModel creates a bi-directional mapping between position and value.
    // That means a change and "position" will trigger a change in "value" and
    // vice-versa.
    // There are however situations where neither "position" nor "value" were
    // changed directly but one of those must be changed to keep the mapping
    // consistent. In this situations we defined that "value" takes precedence
    // over "position" and that the latter is the one that must change.
    // This fits better real-world use cases, for instance, resizing a volume
    // slider may cause the knob to move (position changes) but the volume must
    // be constant (value is stable).

    QRangeModel m;

    // Init
    m.setRange(0, 10);
    m.setPositionRange(0, 200);
    m.setValue(3);

    // Start watching for signals
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    QCOMPARE(m.value(), 3.0);
    QCOMPARE(m.position(), 60.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 0);

    // Change position range. Value keeps constant
    m.setPositionRange(0, 100);

    QCOMPARE(m.value(), 3.0);
    QCOMPARE(m.position(), 30.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.at(0).at(0).toReal(), 30.0);

    // Change position range to inverted figures. Value keeps constant.
    m.setPositionRange(-10, -20);

    QCOMPARE(m.value(), 3.0);
    QCOMPARE(m.position(), -13.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.at(1).at(0).toReal(), -13.0);

    // Change value range. Value keeps constant since it's still in range.
    m.setRange(3, 4);

    QCOMPARE(m.value(), 3.0);
    QCOMPARE(m.position(), -10.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionChangedSpy.at(2).at(0).toReal(), -10.0);

    // Change position range. This time position is kept constant because
    // positionAtMinimum does not change "value" happens to be at minimum.
    m.setPositionAtMaximum(0);

    QCOMPARE(m.value(), 3.0);
    QCOMPARE(m.position(), -10.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 3);
}

void tst_QRangeModel::outOfRangeValues()
{
    QRangeModel m;

    // Init with value and position inside range
    m.setRange(50, 100);
    m.setPositionRange(200, 300);
    m.setValue(80);

    // Start watching for signals
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    int valueSignalsCount = 0;
    int positionSignalsCount = 0;
    qreal expectedValue = 0;
    qreal expectedPosition = 0;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Set out of range value, visible value and position should move to border
    m.setValue(20);

    valueSignalsCount++;
    positionSignalsCount++;
    expectedValue = 50;
    expectedPosition = 200;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(valueChangedSpy.at(valueSignalsCount - 1).at(0).toReal(), expectedValue);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);

    // Set other out of range value, visible value and position should not change
    m.setValue(30);

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Set one more out of range value, visible value and position should not change
    m.setValue(40);

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Make range broad. External value must change.
    m.setRange(25, 100);

    valueSignalsCount++;
    positionSignalsCount++;
    expectedValue = 40;
    expectedPosition = 220;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(valueChangedSpy.at(valueSignalsCount - 1).at(0).toReal(), expectedValue);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);

    // Close range. External value must change.
    m.setRange(60, 100);

    valueSignalsCount++;
    positionSignalsCount++;
    expectedValue = 60;
    expectedPosition = 200;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(valueChangedSpy.at(valueSignalsCount - 1).at(0).toReal(), expectedValue);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);

    // Set value right at the edge. Internal value must change. External one, not.
    m.setValue(60);

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Make range broad. External value does not change. Position does.
    m.setRange(20, 120);

    positionSignalsCount++;
    expectedPosition = 240;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);
}

void tst_QRangeModel::outOfRangePositions()
{
    QRangeModel m;

    // Init with value and position inside range
    m.setRange(50, 100);
    m.setPositionRange(200, 300);
    m.setPosition(260);

    // Start watching for signals
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    int valueSignalsCount = 0;
    int positionSignalsCount = 0;
    qreal expectedValue = 0;
    qreal expectedPosition = 0;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Set out of range value, visible value and position should move to border
    m.setPosition(140);

    valueSignalsCount++;
    positionSignalsCount++;
    expectedValue = 50;
    expectedPosition = 200;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(valueChangedSpy.at(valueSignalsCount - 1).at(0).toReal(), expectedValue);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);

    // Set other out of range value, visible value and position should not change
    m.setPosition(160);

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Set one more out of range value, visible value and position should not change
    m.setPosition(180);

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Make range broad. External value must change.
    m.setRange(25, 100);

    valueSignalsCount++;
    positionSignalsCount++;
    expectedValue = 40;
    expectedPosition = 220;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(valueChangedSpy.at(valueSignalsCount - 1).at(0).toReal(), expectedValue);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);

    // Close range. External value must change.
    m.setRange(60, 100);

    valueSignalsCount++;
    positionSignalsCount++;
    expectedValue = 60;
    expectedPosition = 200;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(valueChangedSpy.at(valueSignalsCount - 1).at(0).toReal(), expectedValue);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);

    // Set position right at the edge. Internal value must change. External one, not.
    m.setPosition(200);

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);

    // Make range broad. External value does not change. Position does.
    m.setRange(20, 120);

    positionSignalsCount++;
    expectedPosition = 240;

    QCOMPARE(valueChangedSpy.count(), valueSignalsCount);
    QCOMPARE(positionChangedSpy.count(), positionSignalsCount);
    QCOMPARE(positionChangedSpy.at(positionSignalsCount - 1).at(0).toReal(), expectedPosition);
}

void tst_QRangeModel::valueForPosition_data()
{
    QTest::addColumn<qreal>("position");
    QTest::addColumn<qreal>("value");

    QTest::newRow("Below minimum") << qreal(150.0) << qreal(5.0);
    QTest::newRow("At minimum") << qreal(200.0) << qreal(5.0);
    QTest::newRow("At 1/5") << qreal(220.0) << qreal(6.0);
    QTest::newRow("At 3/5") << qreal(260.0) << qreal(8.0);
    QTest::newRow("At maximum") << qreal(300.0) << qreal(10.0);
    QTest::newRow("Above maximum") << qreal(350.0) << qreal(10.0);
}

void tst_QRangeModel::valueForPosition()
{
    QRangeModel m;

    const qreal minimum = 5;
    const qreal maximum = 10;
    const qreal positionAtMinimum = 200;
    const qreal positionAtMaximum = 300;

    // Init
    m.setRange(minimum, maximum);
    m.setPositionRange(positionAtMinimum, positionAtMaximum);

    QFETCH(qreal, position);
    QFETCH(qreal, value);

    // Base test
    QCOMPARE(m.valueForPosition(position), value);

    // Inverted flag
    m.setInverted(true);
    QCOMPARE(m.valueForPosition(position), maximum - value + minimum);

    // Inverted boundaries and Inverted flag
    m.setPositionRange(positionAtMaximum, positionAtMinimum);
    QCOMPARE(m.valueForPosition(position), value);

    // Inverted boundaries only
    m.setInverted(false);
    QCOMPARE(m.valueForPosition(position), maximum - value + minimum);
}

void tst_QRangeModel::positionForValue_data()
{
    QTest::addColumn<qreal>("position");
    QTest::addColumn<qreal>("value");

    QTest::newRow("Below minimum") << qreal(200.0) << qreal(4.0);
    QTest::newRow("At minimum") << qreal(200.0) << qreal(5.0);
    QTest::newRow("At 1/5") << qreal(220.0) << qreal(6.0);
    QTest::newRow("At 3/5") << qreal(260.0) << qreal(8.0);
    QTest::newRow("At maximum") << qreal(300.0) << qreal(10.0);
    QTest::newRow("Above maximum") << qreal(300.0) << qreal(12.0);
}

void tst_QRangeModel::positionForValue()
{
    QRangeModel m;

    const qreal minimum = 5;
    const qreal maximum = 10;
    const qreal positionAtMinimum = 200;
    const qreal positionAtMaximum = 300;

    // Init
    m.setRange(minimum, maximum);
    m.setPositionRange(positionAtMinimum, positionAtMaximum);

    QFETCH(qreal, position);
    QFETCH(qreal, value);

    // Base test
    QCOMPARE(m.positionForValue(value), position);

    // Inverted flag
    m.setInverted(true);
    QCOMPARE(m.positionForValue(value), positionAtMaximum - position + positionAtMinimum);

    // Inverted boundaries and Inverted flag
    m.setPositionRange(positionAtMaximum, positionAtMinimum);
    QCOMPARE(m.positionForValue(value), position);

    // Inverted boundaries only
    m.setInverted(false);
    QCOMPARE(m.positionForValue(value), positionAtMaximum - position + positionAtMinimum);
}

void tst_QRangeModel::stepSize()
{
    QRangeModel m;

    // Value 0 makes RangeModelb behaves like ifit were Analog
    m.setRange(0.0, 50.0);
    m.setPositionRange(0.0, 50.0);
    m.setValue(0.0);
    m.setStepSize(10.0); // 0--10--20--30--40--50

    QSignalSpy valueSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionSpy(&m, SIGNAL(positionChanged(qreal)));
    QSignalSpy stepSizeSpy(&m, SIGNAL(stepSizeChanged(qreal)));

    // Value and position by changing value
    m.setValue(10.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 10.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);
    QCOMPARE(valueSpy.at(0).at(0).toReal(), 10.0); // Must not emit a signal with the internal value
    QCOMPARE(positionSpy.at(0).at(0).toReal(), 10.0);

    m.setValue(33.0);
    QCOMPARE(m.value(), 30.0); // Rounds to the closest value of the value set +/- stepSize
    QCOMPARE(m.position(), 30.0);
    QCOMPARE(valueSpy.count(), 2);
    QCOMPARE(positionSpy.count(), 2);
    QCOMPARE(valueSpy.at(1).at(0).toReal(), 30.0); // Must not emit a signal with the internal value
    QCOMPARE(positionSpy.at(1).at(0).toReal(), 30.0);

    m.setValue(26.0);
    QCOMPARE(m.value(), 30.0);
    QCOMPARE(m.position(), 30.0);

    // Value and position by changing position
    m.setPosition(23.0);
    QCOMPARE(m.value(), 20.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueSpy.count(), 3);
    QCOMPARE(positionSpy.count(), 3);
    QCOMPARE(valueSpy.at(2).at(0).toReal(), 20.0); // Must not emit a signal with the internal value
    QCOMPARE(positionSpy.at(2).at(0).toReal(), 20.0);

    m.setPosition(48.0);
    QCOMPARE(m.value(), 50.0);
    QCOMPARE(m.position(), 50.0);
    QCOMPARE(valueSpy.count(), 4);
    QCOMPARE(positionSpy.count(), 4);
    QCOMPARE(valueSpy.at(3).at(0).toReal(), 50.0); // Must not emit a signal with the internal value
    QCOMPARE(positionSpy.at(3).at(0).toReal(), 50.0);

    m.setValue(23.0);

    m.setStepSize(5.0);
    QCOMPARE(m.value(), 25.0); // When the step size is reset the set value is also reseted
    QCOMPARE(m.position(), 25.0);
    QCOMPARE(valueSpy.count(), 6);
    QCOMPARE(positionSpy.count(), 6);
    QCOMPARE(stepSizeSpy.count(), 1);
    QCOMPARE(valueSpy.at(5).at(0).toReal(), 25.0); // Must not emit a signal with the internal value
    QCOMPARE(positionSpy.at(5).at(0).toReal(), 25.0);

    // Range size not multiple of stepSize
    m.setRange(0.0, 100.0);
    QCOMPARE(m.value(), 25.0);
    QCOMPARE(m.position(), 12.5);
    QCOMPARE(valueSpy.count(), 6);
    QCOMPARE(positionSpy.count(), 7);
    QCOMPARE(positionSpy.at(6).at(0).toReal(), 12.5);

    m.setStepSize(30.0); // 0--30--60--90--100
    QCOMPARE(m.value(), 30.0);
    QCOMPARE(m.position(), 15.0);
    QCOMPARE(valueSpy.count(), 7);
    QCOMPARE(positionSpy.count(), 8);
    QCOMPARE(stepSizeSpy.count(), 2);
    QCOMPARE(valueSpy.at(6).at(0).toReal(), 30.0);
    QCOMPARE(positionSpy.at(7).at(0).toReal(), 15.0);

    m.setValue(96.0);
    QCOMPARE(m.value(), 100.0); // Closer value
    QCOMPARE(m.position(), 50.0);
    QCOMPARE(valueSpy.count(), 8);
    QCOMPARE(positionSpy.count(), 9);
    QCOMPARE(valueSpy.at(7).at(0).toReal(), 100.0);
    QCOMPARE(positionSpy.at(8).at(0).toReal(), 50.0);

    m.setValue(94.0);
    QCOMPARE(m.value(), 90.0);
    QCOMPARE(m.position(), 45.0);
    QCOMPARE(valueSpy.count(), 9);
    QCOMPARE(positionSpy.count(), 10);
    QCOMPARE(valueSpy.at(8).at(0).toReal(), 90.0);
    QCOMPARE(positionSpy.at(9).at(0).toReal(), 45.0);

    // Supressing unnecessary signals
    m.setPositionRange(0.0, 20.0);
    QCOMPARE(m.value(), 90.0);
    QCOMPARE(m.position(), 18.0);
    QCOMPARE(valueSpy.count(), 9);
    QCOMPARE(positionSpy.count(), 11);
    QCOMPARE(positionSpy.at(10).at(0).toReal(), 18.0);

    m.setValue(0.0);
    QCOMPARE(m.value(), 0.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueSpy.count(), 10);
    QCOMPARE(positionSpy.count(), 12);
    QCOMPARE(valueSpy.at(9).at(0).toReal(), 0.0);
    QCOMPARE(positionSpy.at(11).at(0).toReal(), 0.0);

    m.setStepSize(10.0);
    QCOMPARE(m.stepSize(), 10.0);
    QCOMPARE(stepSizeSpy.count(), 3);

    valueSpy.clear();
    positionSpy.clear();

    m.setValue(10.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);
    QCOMPARE(valueSpy.at(0).at(0).toReal(), 10.0);
    QCOMPARE(positionSpy.at(0).at(0).toReal(), 2.0);

    m.setValue(11.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);

    m.setValue(12.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);

    m.setValue(13.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);

    m.setValue(14.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);

    m.setValue(15.0);
    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 2.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);

    m.setValue(15.1);
    QCOMPARE(m.value(), 20.0);
    QCOMPARE(m.position(), 4.0);
    QCOMPARE(valueSpy.count(), 2);
    QCOMPARE(positionSpy.count(), 2);
    QCOMPARE(valueSpy.at(1).at(0).toReal(), 20.0);
    QCOMPARE(positionSpy.at(1).at(0).toReal(), 4.0);

    // StepSize 0
    m.setValue(15.0);
    m.setStepSize(0.0);
    QCOMPARE(m.value(), 15.0);
    QCOMPARE(m.position(), 3.0);
    QCOMPARE(valueSpy.count(), 4);
    QCOMPARE(positionSpy.count(), 4);
    QCOMPARE(stepSizeSpy.count(), 4);
    QCOMPARE(valueSpy.at(3).at(0).toReal(), 15.0);
    QCOMPARE(positionSpy.at(3).at(0).toReal(), 3.0);

    m.setValue(12.0);
    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), 2.4);
    QCOMPARE(valueSpy.count(), 5);
    QCOMPARE(positionSpy.count(), 5);
    QCOMPARE(valueSpy.at(4).at(0).toReal(), 12.0);
    QCOMPARE(positionSpy.at(4).at(0).toReal(), 2.4);

    m.setPosition(0.2);
    QCOMPARE(m.value(), 1.0);
    QCOMPARE(m.position(), 0.2);
    QCOMPARE(valueSpy.count(), 6);
    QCOMPARE(positionSpy.count(), 6);
    QCOMPARE(valueSpy.at(5).at(0).toReal(), 1.0);
    QCOMPARE(positionSpy.at(5).at(0).toReal(), 0.2);

    // StepSize bigger than range
    m.setStepSize(300.0);
    QCOMPARE(m.stepSize(), 300.0);
    QCOMPARE(m.value(), 0.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueSpy.count(), 7);
    QCOMPARE(valueSpy.count(), 7);
    QCOMPARE(stepSizeSpy.count(), 5);
    QCOMPARE(valueSpy.at(6).at(0).toReal(), 0.0);
    QCOMPARE(positionSpy.at(6).at(0).toReal(), 0.0);

    m.setValue(50.0);
    QCOMPARE(m.value(), 0.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueSpy.count(), 7);
    QCOMPARE(positionSpy.count(), 7);
    //QCOMPARE(stepSizeSpy.at(0).at(0), qreal(100)); // Max range

    m.setValue(51.0);
    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueSpy.count(), 8);
    QCOMPARE(positionSpy.count(), 8);
    QCOMPARE(valueSpy.at(7).at(0).toReal(), 100.0);
    QCOMPARE(positionSpy.at(7).at(0).toReal(), 20.0);

    m.setStepSize(100.0);
    QCOMPARE(m.stepSize(), 100.0);
    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(stepSizeSpy.count(), 6);

    m.setValue(50.0);
    QCOMPARE(m.value(), 0.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueSpy.count(), 9);
    QCOMPARE(positionSpy.count(), 9);
    QCOMPARE(valueSpy.at(8).at(0).toReal(), 0.0);
    QCOMPARE(positionSpy.at(8).at(0).toReal(), 0.0);

    m.setStepSize(300.0);
    QCOMPARE(valueSpy.count(), 9);
    QCOMPARE(positionSpy.count(), 9);

    m.setRange(-100.0, 100.0);
    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueSpy.count(), 10);
    QCOMPARE(positionSpy.count(), 10);
    QCOMPARE(valueSpy.at(9).at(0).toReal(), 100.0);
    QCOMPARE(positionSpy.at(9).at(0).toReal(), 20.0);

    m.setValue(0.0);
    QCOMPARE(m.value(), -100.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueSpy.count(), 11);
    QCOMPARE(positionSpy.count(), 11);
    QCOMPARE(valueSpy.at(10).at(0).toReal(), -100.0);
    QCOMPARE(positionSpy.at(10).at(0).toReal(), 0.0);

    m.setValue(1.0);
    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueSpy.count(), 12);
    QCOMPARE(positionSpy.count(), 12);
    QCOMPARE(valueSpy.at(11).at(0).toReal(), 100.0);
    QCOMPARE(positionSpy.at(11).at(0).toReal(), 20.0);
    //QCOMPARE(stepSizeSpy.at(1).at(0), qreal(200)); // Max range

    QCOMPARE(m.stepSize(), 300.0);

    m.setRange(0.0, 100.0);
    m.setStepSize(99.0);
    QCOMPARE(m.stepSize(), 99.0);
    QCOMPARE(stepSizeSpy.count(), 8);

    stepSizeSpy.clear();
    valueSpy.clear();
    positionSpy.clear();

    m.setValue(99.0);
    QCOMPARE(m.value(), 99.0);
    QCOMPARE(m.position(), 19.8);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(positionSpy.count(), 1);
    QCOMPARE(stepSizeSpy.count(), 0);
    QCOMPARE(valueSpy.at(0).at(0).toReal(), 99.0);
    QCOMPARE(positionSpy.at(0).at(0).toReal(), 19.8);

    m.setValue(99.51);
    QCOMPARE(m.value(), 100.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueSpy.count(), 2);
    QCOMPARE(positionSpy.count(), 2);
    QCOMPARE(stepSizeSpy.count(), 0);
    QCOMPARE(valueSpy.at(1).at(0).toReal(), 100.0);
    QCOMPARE(positionSpy.at(1).at(0).toReal(), 20.0);

    m.setValue(5000);
    QCOMPARE(m.value(), 100.0);
}

void tst_QRangeModel::invertedStepSize()
{
    QRangeModel m;
    m.setRange(40, 41);
    m.setPositionRange(250, 200);

    m.setStepSize(0.3);

    QCOMPARE(m.value(), 40.0);
    QCOMPARE(m.position(), 250.0);

    m.setPosition(225);
    QCOMPARE(m.position(), 220.0);

    m.setPosition(203);
    QCOMPARE(m.position(), 205.0);

    m.setPosition(360);
    QCOMPARE(m.position(), 250.0);

    m.setPosition(130);
    QCOMPARE(m.position(), 200.0);
}

void tst_QRangeModel::settingOwnPositionAndValue()
{
    QRangeModel m;
    m.setRange(0.0, 100.0);

    // This range for position will cause the step to be non-integer, the actual
    // figures came from a test of a Slider component.
    m.setPositionRange(32.0, 268.0);
    m.setStepSize(1.0);
    m.setPosition(32.0);

    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));
    m.setPosition(215);
    QCOMPARE(m.position(), 216.08);
    m.setPosition(m.position());
    QCOMPARE(m.position(), 216.08);
    QCOMPARE(positionChangedSpy.count(), 1);

    // Now with a step size that will cause non-integer values
    m.setStepSize(1 + (1.0 / 3.0));
    m.setValue(0.0);

    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    m.setValue(7.9);
    QCOMPARE(m.value(), 8.0);
    m.setValue(m.value());
    QCOMPARE(m.value(), 8.0);
    QCOMPARE(valueChangedSpy.count(), 1);
}

QTEST_MAIN(tst_QRangeModel)
#include "tst_qrangemodel.moc"
