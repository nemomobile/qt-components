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
};

tst_QRangeModel::tst_QRangeModel()
{
}

tst_QRangeModel::~tst_QRangeModel()
{
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

QTEST_MAIN(tst_QRangeModel)
#include "tst_qrangemodel.moc"
