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

#include "tst_quickcomponentstest.h"
#include <QtTest/QtTest>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>

class tst_SDeclarativeStyle : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testPlatformStyle_data();
    void testPlatformStyle();
    void testPrivateStyle_data();
    void testPrivateStyle();

private:
    void verifyStyleData(const QString &param, QVariant::Type type, QObject *style);

private:
    QDeclarativeView *window;
    QObject *platformStyle;
    QObject *privateStyle;
};

void tst_SDeclarativeStyle::initTestCase()
{
    window = tst_quickcomponentstest::createDeclarativeView("tst_declarativestyle.qml");
    window->show();
    QTest::qWaitForWindowShown(window);
    platformStyle = qVariantValue<QObject *>(window->engine()->rootContext()->contextProperty("platformStyle"));
    QVERIFY(platformStyle);
    privateStyle = qVariantValue<QObject *>(window->engine()->rootContext()->contextProperty("privateStyle"));
    QVERIFY(privateStyle);
}

void tst_SDeclarativeStyle::cleanupTestCase()
{
    delete window;
    window = 0;
}

void tst_SDeclarativeStyle::testPlatformStyle_data()
{
    QTest::addColumn<QString>("param");
    QTest::addColumn<int>("type");

    // font params
    QTest::newRow("fontFamilyRegular") << "fontFamilyRegular" << (int)QVariant::String;

    // layout params
    QTest::newRow("fontSizeLarge") << "fontSizeLarge" << (int)QVariant::Int;
    QTest::newRow("fontSizeMedium") << "fontSizeMedium" << (int)QVariant::Int;
    QTest::newRow("fontSizeSmall") << "fontSizeSmall" << (int)QVariant::Int;
    QTest::newRow("graphicSizeLarge") << "graphicSizeLarge" << (int)QVariant::Int;
    QTest::newRow("graphicSizeMedium") << "graphicSizeMedium" << (int)QVariant::Int;
    QTest::newRow("graphicSizeSmall") << "graphicSizeSmall" << (int)QVariant::Int;
    QTest::newRow("graphicSizeTiny") << "graphicSizeTiny" << (int)QVariant::Int;
    QTest::newRow("paddingLarge") << "paddingLarge" << (int)QVariant::Int;
    QTest::newRow("paddingMedium") << "paddingMedium" << (int)QVariant::Int;
    QTest::newRow("paddingSmall") << "paddingSmall" << (int)QVariant::Int;
    QTest::newRow("borderSizeMedium") << "borderSizeMedium" << (int)QVariant::Int;

    // color params
    QTest::newRow("colorNormalLight") << "colorNormalLight" << (int)QVariant::Color;
    QTest::newRow("colorNormalMid") << "colorNormalMid" << (int)QVariant::Color;
    QTest::newRow("colorNormalDark") << "colorNormalDark" << (int)QVariant::Color;
    QTest::newRow("colorNormalLink") << "colorNormalLink" << (int)QVariant::Color;
    QTest::newRow("colorPressed") << "colorPressed" << (int)QVariant::Color;
    QTest::newRow("colorChecked") << "colorChecked" << (int)QVariant::Color;
    QTest::newRow("colorHighlighted") << "colorHighlighted" << (int)QVariant::Color;
    QTest::newRow("colorDisabledLight") << "colorDisabledLight" << (int)QVariant::Color;
    QTest::newRow("colorDisabledMid") << "colorDisabledMid" << (int)QVariant::Color;
    QTest::newRow("colorDisabledDark") << "colorDisabledDark" << (int)QVariant::Color;
    QTest::newRow("colorTextSelection") << "colorTextSelection" << (int)QVariant::Color;

    // invalid param
    QTest::newRow("__invalid__") << "__invalid__" << (int)QVariant::Invalid;
}

void tst_SDeclarativeStyle::testPlatformStyle()
{
    QFETCH(QString, param);
    QFETCH(int, type);
    verifyStyleData(param, (QVariant::Type)type, platformStyle);
}

void tst_SDeclarativeStyle::testPrivateStyle_data()
{
    QTest::addColumn<QString>("param");
    QTest::addColumn<int>("type");

    // layout params
    QTest::newRow("statusBarHeight") << "statusBarHeight" << (int)QVariant::Int;
    QTest::newRow("tabBarHeightPortrait") << "tabBarHeightPortrait" << (int)QVariant::Int;
    QTest::newRow("tabBarHeightLandscape") << "tabBarHeightLandscape" << (int)QVariant::Int;
    QTest::newRow("toolBarHeightPortrait") << "toolBarHeightPortrait" << (int)QVariant::Int;
    QTest::newRow("toolBarHeightLandscape") << "toolBarHeightLandscape" << (int)QVariant::Int;
    QTest::newRow("scrollBarThickness") << "scrollBarThickness" << (int)QVariant::Int;
    QTest::newRow("sliderThickness") << "sliderThickness" << (int)QVariant::Int;
    QTest::newRow("menuItemHeight") << "menuItemHeight" << (int)QVariant::Int;
    QTest::newRow("textFieldHeight") << "textFieldHeight" << (int)QVariant::Int;
    QTest::newRow("switchButtonHeight") << "switchButtonHeight" << (int)QVariant::Int;
    QTest::newRow("dialogMinSize") << "dialogMinSize" << (int)QVariant::Int;
    QTest::newRow("dialogMaxSize") << "dialogMaxSize" << (int)QVariant::Int;
    QTest::newRow("listItemSeparatorColor") << "listItemSeparatorColor" << (int)QVariant::Color;
    QTest::newRow("buttonSize") << "buttonSize" << (int)QVariant::Int;

    // invalid param
    QTest::newRow("__invalid__") << "__invalid__" << (int)QVariant::Invalid;
}

void tst_SDeclarativeStyle::testPrivateStyle()
{
    QFETCH(QString, param);
    QFETCH(int, type);
    verifyStyleData(param, (QVariant::Type)type, privateStyle);
}

void tst_SDeclarativeStyle::verifyStyleData(const QString &param, QVariant::Type type, QObject *style)
{
    QVariant value = style->property(param.toAscii().constData());
    QCOMPARE(value.type(), type);
    if (type == QVariant::String) {
        QVERIFY(!value.toString().isNull());
    } else if (type == QVariant::Int) {
        QVERIFY(value.toInt() >= 0);
    } else if (type == QVariant::Color) {
        QVERIFY(value.value<QColor>().isValid());
    }
}

QTEST_MAIN(tst_SDeclarativeStyle)

#include "tst_declarativestyle.moc"
