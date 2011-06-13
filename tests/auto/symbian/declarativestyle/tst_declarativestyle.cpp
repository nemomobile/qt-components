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
    QTest::newRow("colorBackground") << "colorBackground" << (int)QVariant::Color;
    QTest::newRow("colorNormalLight") << "colorNormalLight" << (int)QVariant::Color;
    QTest::newRow("colorNormalMid") << "colorNormalMid" << (int)QVariant::Color;
    QTest::newRow("colorNormalDark") << "colorNormalDark" << (int)QVariant::Color;
    QTest::newRow("colorNormalLink") << "colorNormalLink" << (int)QVariant::Color;
    QTest::newRow("colorPressed") << "colorPressed" << (int)QVariant::Color;
    QTest::newRow("colorLatched") << "colorLatched" << (int)QVariant::Color;
    QTest::newRow("colorHighlighted") << "colorHighlighted" << (int)QVariant::Color;
    QTest::newRow("colorDisabledLight") << "colorDisabledLight" << (int)QVariant::Color;
    QTest::newRow("colorDisabledMid") << "colorDisabledMid" << (int)QVariant::Color;
    QTest::newRow("colorDisabledDark") << "colorDisabledDark" << (int)QVariant::Color;
    QTest::newRow("colorTextSelection") << "colorTextSelection" << (int)QVariant::Color;
    QTest::newRow("colorBackgroundInverted") << "colorBackgroundInverted" << (int)QVariant::Color;
    QTest::newRow("colorNormalLightInverted") << "colorNormalLightInverted" << (int)QVariant::Color;
    QTest::newRow("colorNormalMidInverted") << "colorNormalMidInverted" << (int)QVariant::Color;
    QTest::newRow("colorNormalDarkInverted") << "colorNormalDarkInverted" << (int)QVariant::Color;
    QTest::newRow("colorNormalLinkInverted") << "colorNormalLinkInverted" << (int)QVariant::Color;
    QTest::newRow("colorPressedInverted") << "colorPressedInverted" << (int)QVariant::Color;
    QTest::newRow("colorLatchedInverted") << "colorLatchedInverted" << (int)QVariant::Color;
    QTest::newRow("colorHighlightedInverted") << "colorHighlightedInverted" << (int)QVariant::Color;
    QTest::newRow("colorDisabledLightInverted") << "colorDisabledLightInverted" << (int)QVariant::Color;
    QTest::newRow("colorDisabledMidInverted") << "colorDisabledMidInverted" << (int)QVariant::Color;
    QTest::newRow("colorDisabledDarkInverted") << "colorDisabledDarkInverted" << (int)QVariant::Color;
    QTest::newRow("colorTextSelectionInverted") << "colorTextSelectionInverted" << (int)QVariant::Color;

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
