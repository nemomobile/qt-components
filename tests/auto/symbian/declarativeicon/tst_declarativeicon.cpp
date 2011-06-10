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

#include "sdeclarativeicon.h"
#include <QtTest/QtTest>
#include <QUrl>
#include <QImage>
#include <QPainter>

bool fuzzyCompareImages(const QImage &image1, const QImage &image2, int tolerance)
{
    if (image1.bytesPerLine() != image2.bytesPerLine()
        || image1.width() != image2.width()
        || image1.height() != image2.height()) {
        return false;
    }

    // E.g. (0,10) and (10,0) sizes are treated similar.
    if ((!image1.width() || !image1.height())
        && (!image2.width() || !image2.height())) {
        return true;
    }
    bool same = true;
    for (int i = 0; i < image1.height(); i++) {
        const uchar *line1 = image1.scanLine(i);
        const uchar *line2 = image2.scanLine(i);

        int bytes = image1.bytesPerLine();
        for (int j = 0; j < bytes ; j++) {
            int delta = line1[j] - line2[j];
            if (qAbs(delta) > tolerance) {
                same = false;
                break;
            }
        }
        if (!same) {
            break;
        }
    }
    return same;
}

void doPaint(QPainter *painter, SDeclarativeIcon *icon)
{
    painter->eraseRect(painter->viewport());
    icon->paint(painter, 0, 0);
}

class TestDeclarativeIcon : public SDeclarativeIcon
{
    Q_OBJECT

public:
    TestDeclarativeIcon() : SDeclarativeIcon(),
                            iconNameChanged(false),
                            iconColorChanged(false)
    {
        connect(this, SIGNAL(iconNameChanged(QString)), this, SLOT(handleIconNameChanged()));
        connect(this, SIGNAL(iconColorChanged(QColor)), this, SLOT(handleColorChanged()));
    }

    void setSize(int width, int height)
    {
        SDeclarativeIcon::setImplicitWidth(width);
        SDeclarativeIcon::setImplicitHeight(height);
    }

    bool iconNameChanged;
    bool iconColorChanged;

private slots:
    void handleIconNameChanged()
    {
        iconNameChanged = true;
    }

    void handleColorChanged()
    {
        iconColorChanged = true;
    }
};

class tst_SDeclarativeIcon : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testIcon_constructor();
    void testIconiconName();
    void testIcon_setIconName();
    void testIcon_color();
    void testIcon_setColor();
    void testIcon_paint();
    void testIcon_signals();
};

void tst_SDeclarativeIcon::init()
{
}

void tst_SDeclarativeIcon::cleanup()
{
}
void tst_SDeclarativeIcon::initTestCase()
{
}

void tst_SDeclarativeIcon::cleanupTestCase()
{
}

void tst_SDeclarativeIcon::testIcon_constructor()
{
    SDeclarativeIcon icon;
}

void tst_SDeclarativeIcon::testIconiconName()
{
    SDeclarativeIcon icon;
    QVERIFY(icon.iconName().isEmpty());
}

void tst_SDeclarativeIcon::testIcon_setIconName()
{
    SDeclarativeIcon icon;
    icon.setIconName("test");
    QCOMPARE(icon.iconName(), QString("test"));
}

void tst_SDeclarativeIcon::testIcon_color()
{
    SDeclarativeIcon icon;
    QCOMPARE(icon.iconColor(), QColor());
}

void tst_SDeclarativeIcon::testIcon_setColor()
{
    SDeclarativeIcon icon;
    icon.setIconColor(QColor("red"));
    QCOMPARE(icon.iconColor(), QColor("red"));
}

void tst_SDeclarativeIcon::testIcon_paint()
{
    QImage image1(100, 100, QImage::Format_ARGB32);
    QPainter painter1(&image1);
    QImage image2(100, 100, QImage::Format_ARGB32);
    QPainter painter2(&image2);

    TestDeclarativeIcon icon;
    icon.setIconName("qtg_graf_radiobutton_normal_selected");
    icon.setSize(100, 100);

    // Paint to both painters to verify compareImages function works
    doPaint(&painter1, &icon);
    doPaint(&painter2, &icon);

    // Same icons, should be same result
    QVERIFY(fuzzyCompareImages(image1, image2, 0));

    // Keep same size but change icon name
    icon.setIconName("qtg_graf_radiobutton_normal_unselected");

    doPaint(&painter2, &icon);
    QVERIFY(!fuzzyCompareImages(image1, image2, 0));

    // Change size smaller
    icon.setIconName("qtg_graf_radiobutton_normal_selected");
    icon.setSize(95, 95);

    doPaint(&painter2, &icon);
    // Same icons but different size, should be different result
    QVERIFY(!fuzzyCompareImages(image1, image2, 0));

    // Use a "mono" icon
    icon.setIconName("qtg_graf_drill_down_indicator");

    doPaint(&painter1, &icon);
    icon.setIconColor(QColor("blue"));
    doPaint(&painter2, &icon);
    // Different color so should be different result
    QVERIFY(!fuzzyCompareImages(image1, image2, 0));
}

void tst_SDeclarativeIcon::testIcon_signals()
{
    TestDeclarativeIcon icon;
    icon.setIconName("qtg_graf_radiobutton_normal_selected");
    // Should have triggered signal
    QVERIFY(icon.iconNameChanged);

    icon.setIconColor(QColor("red"));
    QVERIFY(icon.iconColorChanged);

    // Try setting properties to same values, that should not trigger signals
    icon.iconNameChanged = false;
    icon.iconColorChanged = false;

    icon.setIconName("qtg_graf_radiobutton_normal_selected");
    QVERIFY(!icon.iconNameChanged);

    icon.setIconColor(QColor("red"));
    QVERIFY(!icon.iconColorChanged);

    // Try changing values to something different, that should trigger signals again
    icon.iconNameChanged = false;
    icon.iconColorChanged = false;

    icon.setIconName("qtg_graf_radiobutton_normal_unselected");
    QVERIFY(icon.iconNameChanged);

    icon.setIconColor(QColor("blue"));
    QVERIFY(icon.iconColorChanged);
}
QTEST_MAIN(tst_SDeclarativeIcon)

#include "tst_declarativeicon.moc"
