/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "qrangemodel.h"

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
    void toMaximum();
    void toMinimum();
    void invertedTest();
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

    m.toMinimum();
    QCOMPARE(m.value(), 50.0);
    QCOMPARE(m.position(), 0.0);

    //### NEW RANGE
    m.setRange(0.0, 200.0);

    QCOMPARE(m.value(), 50.0);
    QCOMPARE(m.position(), 25.0);
    QCOMPARE(positionChangedSpy.count(), 1);
    args = positionChangedSpy.at(0);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMinimum();
    QCOMPARE(m.value(), 0.0);
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    args = valueChangedSpy.at(1);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(positionChangedSpy.count(), 2);
    args = positionChangedSpy.at(1);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(100.0, 150.0);

    QCOMPARE(m.value(), 100.0);
    QCOMPARE(valueChangedSpy.count(), 3);
    args = valueChangedSpy.at(2);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 2);

    //### NEW RANGE
    m.setRange(-100.0, 300.0);

    QCOMPARE(m.value(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    QCOMPARE(m.position(), 25.0);
    QCOMPARE(positionChangedSpy.count(), 3);
    args = positionChangedSpy.at(2);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMinimum();
    QCOMPARE(m.value(), -100.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    args = valueChangedSpy.at(4);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 4);
    args = positionChangedSpy.at(3);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(-200.0, -100.0);

    QCOMPARE(m.value(), -100.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 5);
    args = positionChangedSpy.at(4);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.toMinimum();
    QCOMPARE(m.value(), -200.0);
    QCOMPARE(valueChangedSpy.count(), 6);
    args = valueChangedSpy.at(5);
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
    m.setInverted(true);

    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));
    QList<QVariant> args;

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
    QCOMPARE(valueChangedSpy.count(), 3);
    QCOMPARE(m.position(), 100.0);
    QCOMPARE(positionChangedSpy.count(), 3);

    //### NEW RANGE
    m.setRange(-100.0, 0.0);

    QCOMPARE(m.value(), 0.0);
    QCOMPARE(valueChangedSpy.count(), 4);
    args = valueChangedSpy.at(3);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 0.0);
    QCOMPARE(positionChangedSpy.count(), 4);
    args = positionChangedSpy.at(3);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setValue(-75.0);
    QCOMPARE(m.value(), -75.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    args = valueChangedSpy.at(4);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 75.0);
    QCOMPARE(positionChangedSpy.count(), 5);
    args = positionChangedSpy.at(4);
    QCOMPARE(m.position(), args.at(0).toReal());

    //### NEW RANGE
    m.setRange(-100.0, -50.0);

    QCOMPARE(m.value(), -75.0);
    QCOMPARE(valueChangedSpy.count(), 5);
    QCOMPARE(m.position(), 50.0);
    QCOMPARE(positionChangedSpy.count(), 6);
    args = positionChangedSpy.at(5);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setValue(-90.0);
    QCOMPARE(m.value(), -90.0);
    QCOMPARE(valueChangedSpy.count(), 6);
    args = valueChangedSpy.at(5);
    QCOMPARE(m.value(), args.at(0).toReal());
    QCOMPARE(m.position(), 80.0);
    QCOMPARE(positionChangedSpy.count(), 7);
    args = positionChangedSpy.at(6);
    QCOMPARE(m.position(), args.at(0).toReal());

    m.setInverted(false);

    QCOMPARE(m.value(), -90.0);
    QCOMPARE(valueChangedSpy.count(), 6);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(positionChangedSpy.count(), 8);
    args = positionChangedSpy.at(7);
    QCOMPARE(m.position(), args.at(0).toReal());
}

void tst_QRangeModel::valueAndPosition_data()
{
    QTest::addColumn<qreal>("min");
    QTest::addColumn<qreal>("max");
    QTest::addColumn<qreal>("posAtMin");
    QTest::addColumn<qreal>("posAtMax");
    // QTest::addColumn<RealList>("values");
    // QTest::addColumn<RealList>("positions");

    QTest::newRow("same range") << 1.0 << 2.0 << 1.0 << 2.0;
    QTest::newRow("same range inverted pos") << 1.0 << 2.0 << 2.0 << 1.0;
    QTest::newRow("pos: 0 100 -> value: 1 2") << 1.0 << 2.0 << 0.0 << 100.0;
    QTest::newRow("pos: 100 0 -> value: 1 2") << 1.0 << 2.0 << 100.0 << 0.0;
    QTest::newRow("pos: -200 300 -> value: 1 2") << 1.0 << 2.0 << -200.0 << 300.0;
    QTest::newRow("pos: 300 -200 -> value: -1 2") << -1.0 << 2.0 << 300.0 << -200.0;
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
    QSignalSpy rangeChangedSpy(&m, SIGNAL(rangeChanged(qreal, qreal)));
    QSignalSpy valueChangedSpy(&m, SIGNAL(valueChanged(qreal)));
    QSignalSpy positionChangedSpy(&m, SIGNAL(positionChanged(qreal)));

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), -200.0);
    QCOMPARE(rangeChangedSpy.count(), 0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 0);

    // Standard situation, change range, value keeps constant, position changes
    m.setRange(11, 16);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), -600.0);
    QCOMPARE(rangeChangedSpy.count(), 1);
    QCOMPARE(rangeChangedSpy.at(0).at(0).toReal(), 11.0);
    QCOMPARE(rangeChangedSpy.at(0).at(1).toReal(), 16.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.at(0).at(0).toReal(), -600.0);

    // Test setRange() min boundary. Still, value should not change.
    m.setRange(12, 16);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), -1000.0);
    QCOMPARE(rangeChangedSpy.count(), 2);
    QCOMPARE(rangeChangedSpy.at(1).at(0).toReal(), 12.0);
    QCOMPARE(rangeChangedSpy.at(1).at(1).toReal(), 16.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.at(1).at(0).toReal(), -1000.0);

    // Test setRange() max boundary. Still, value should not change.
    m.setRange(8, 12);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), 1000.0);
    QCOMPARE(rangeChangedSpy.count(), 3);
    QCOMPARE(rangeChangedSpy.at(2).at(0).toReal(), 8.0);
    QCOMPARE(rangeChangedSpy.at(2).at(1).toReal(), 12.0);
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
    QCOMPARE(rangeChangedSpy.count(), 4);
    QCOMPARE(rangeChangedSpy.at(3).at(0).toReal(), 20.0);
    QCOMPARE(rangeChangedSpy.at(3).at(1).toReal(), 30.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0).toReal(), 20.0);
    QCOMPARE(positionChangedSpy.count(), 4);
    QCOMPARE(positionChangedSpy.at(3).at(0).toReal(), -1000.0);

    // Same as above. Now overflowing value()
    m.setRange(-10, -5);

    QCOMPARE(m.value(), -5.0);
    QCOMPARE(m.position(), 1000.0);
    QCOMPARE(rangeChangedSpy.count(), 5);
    QCOMPARE(rangeChangedSpy.at(4).at(0).toReal(), -10.0);
    QCOMPARE(rangeChangedSpy.at(4).at(1).toReal(), -5.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(valueChangedSpy.at(1).at(0).toReal(), -5.0);
    QCOMPARE(positionChangedSpy.count(), 5);
    QCOMPARE(positionChangedSpy.at(4).at(0).toReal(), 1000.0);

    // Now we have a valid range again. Internal value must have survived.
    m.setRange(0, 20);

    QCOMPARE(m.value(), 12.0);
    QCOMPARE(m.position(), 200.0);
    QCOMPARE(rangeChangedSpy.count(), 6);
    QCOMPARE(rangeChangedSpy.at(5).at(0).toReal(), 0.0);
    QCOMPARE(rangeChangedSpy.at(5).at(1).toReal(), 20.0);
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
    QCOMPARE(rangeChangedSpy.count(), 6);
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
    QSignalSpy positionRangeChangedSpy(&m, SIGNAL(positionRangeChanged(qreal, qreal)));

    QCOMPARE(m.value(), 7.0);
    QCOMPARE(m.position(), 90.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 0);
    QCOMPARE(positionRangeChangedSpy.count(), 0);

    // Standard situation. Change position range. Position will change
    m.setPositionRange(-100, 200);

    QCOMPARE(m.value(), 7.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.at(0).at(0).toReal(), 20.0);
    QCOMPARE(positionRangeChangedSpy.count(), 1);
    QCOMPARE(positionRangeChangedSpy.at(0).at(0).toReal(), -100.0);
    QCOMPARE(positionRangeChangedSpy.at(0).at(1).toReal(), 200.0);

    // Corner case situation. Position happens to remain the same.
    m.setPositionRange(-40.0, 110.0);

    QCOMPARE(m.value(), 7.0);
    QCOMPARE(m.position(), 20.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(positionChangedSpy.count(), 1);
    QCOMPARE(positionRangeChangedSpy.count(), 2);
    QCOMPARE(positionRangeChangedSpy.at(1).at(0).toReal(), -40.0);
    QCOMPARE(positionRangeChangedSpy.at(1).at(1).toReal(), 110.0);

    // Tests with value == min
    m.setValue(5);

    QCOMPARE(m.value(), 5.0);
    QCOMPARE(m.position(), -40.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(valueChangedSpy.at(0).at(0).toReal(), 5.0);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.at(1).at(0).toReal(), -40.0);
    QCOMPARE(positionRangeChangedSpy.count(), 2);

    // Changing positionAtMaximum won't cause changes to position nor value
    m.setPositionAtMaximum(160);

    QCOMPARE(m.value(), 5.0);
    QCOMPARE(m.position(), -40.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(positionChangedSpy.count(), 2);
    QCOMPARE(positionRangeChangedSpy.count(), 3);
    QCOMPARE(positionRangeChangedSpy.at(2).at(0).toReal(), -40.0);
    QCOMPARE(positionRangeChangedSpy.at(2).at(1).toReal(), 160.0);

    // Tests with value == max
    m.setValue(10);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 160.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(valueChangedSpy.at(1).at(0).toReal(), 10.0);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionChangedSpy.at(2).at(0).toReal(), 160.0);
    QCOMPARE(positionRangeChangedSpy.count(), 3);

    // Changing positionAtMinimum won't cause changes to position nor value
    m.setPositionAtMinimum(260);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 160.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionRangeChangedSpy.count(), 4);
    QCOMPARE(positionRangeChangedSpy.at(3).at(0).toReal(), 260.0);
    QCOMPARE(positionRangeChangedSpy.at(3).at(1).toReal(), 160.0);

    // Resetting same position range should not change anything
    m.setPositionRange(260, 160);
    m.setPositionAtMinimum(260);
    m.setPositionAtMaximum(160);

    QCOMPARE(m.value(), 10.0);
    QCOMPARE(m.position(), 160.0);
    QCOMPARE(valueChangedSpy.count(), 2);
    QCOMPARE(positionChangedSpy.count(), 3);
    QCOMPARE(positionRangeChangedSpy.count(), 4);
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

QTEST_MAIN(tst_QRangeModel)
#include "tst_qrangemodel.moc"
