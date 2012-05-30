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

#include "sdeclarativeinputcontext.h"
#include "sdeclarativescreen.h"
#include "tst_quickcomponentstest.h"
#include <QTest>
#include <QUrl>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QMainWindow>
#include <QSignalSpy>
#include <QInputContext>

#ifdef Q_OS_SYMBIAN
#include <e32property.h>
const TUint32 KAknFepSoftwareInputpanelHeight = 0x00000005;
const TUid KPSUidAknFep = { 0x100056de };
#endif // Q_OS_SYMBIAN

class tst_SDeclarativeInputContext : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void defaultPropertyValues();
    void height();
    void visible();

private:
    QScopedPointer<QDeclarativeView> m_view;
    QObject *m_inputContext;
};

void tst_SDeclarativeInputContext::initTestCase()
{
    m_view.reset(tst_quickcomponentstest::createDeclarativeView("tst_declarativeinputcontext.qml"));
    QTest::qWaitForWindowShown(m_view.data());
    QVERIFY(m_view.data());
    QVERIFY(m_view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(m_view.data()));
    m_inputContext = qVariantValue<QObject *>(m_view->engine()->rootContext()->contextProperty("inputContext"));
    QVERIFY(m_inputContext);
}

void tst_SDeclarativeInputContext::defaultPropertyValues()
{
    QVERIFY(m_inputContext->property("height").toInt() > 0);
    QCOMPARE(m_inputContext->property("visible").toBool(), false);
}

void tst_SDeclarativeInputContext::height()
{
    // Open VKB
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QVERIFY(textField);
    textField->setFocus(Qt::OtherFocusReason);
    QVERIFY2(QMetaObject::invokeMethod(textField, "openSoftwareInputPanel"), "Could not openSoftwareInputPanel");

    QObject *screen = qVariantValue<QObject *>(m_view->engine()->rootContext()->contextProperty("screen"));
    QVERIFY(screen);

    QVERIFY(screen->property("currentOrientation").toInt() == SDeclarativeScreen::Portrait
         || screen->property("currentOrientation").toInt() == SDeclarativeScreen::PortraitInverted);

    QSignalSpy heightChangedSpy(m_inputContext, SIGNAL(heightChanged()));
    int portraitHeight = m_inputContext->property("height").toInt();

#ifdef Q_OS_SYMBIAN
    QVERIFY(portraitHeight > screen->property("height").toInt() * 0.48);
#else // Q_OS_SYMBIAN
    QCOMPARE(portraitHeight, screen->property("height").toInt() * 1/2);
#endif // Q_OS_SYMBIAN

    //Switch orientation
    screen->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
    QTRY_COMPARE(heightChangedSpy.count(), 1);
    int landscapeHeight = m_inputContext->property("height").toInt();

#ifdef Q_OS_SYMBIAN
    QVERIFY(landscapeHeight > screen->property("height").toInt() * 0.48);
#else // Q_OS_SYMBIAN
    QCOMPARE(landscapeHeight, screen->property("height").toInt() * 1/2);
#endif // Q_OS_SYMBIAN

    QVERIFY(portraitHeight > landscapeHeight);

#ifdef Q_OS_SYMBIAN
    //Switch to portrait
    RProperty::Set( KPSUidAknFep, KAknFepSoftwareInputpanelHeight, 150 );
    screen->setProperty("allowedOrientations", SDeclarativeScreen::Portrait);
    QCoreApplication::sendPostedEvents();
    QTRY_COMPARE(heightChangedSpy.count(), 2);
    QVERIFY(m_inputContext->property("height").toInt() > 350);

    //Switch to landscape
    RProperty::Set( KPSUidAknFep, KAknFepSoftwareInputpanelHeight, 120 );
    screen->setProperty("allowedOrientations", SDeclarativeScreen::Landscape);
    QTRY_COMPARE(heightChangedSpy.count(), 3);
    QVERIFY(m_inputContext->property("height").toInt() > 120);

    //Reset
    RProperty::Set( KPSUidAknFep, KAknFepSoftwareInputpanelHeight, 0 );
    QVERIFY2(QMetaObject::invokeMethod(textField, "closeSoftwareInputPanel"), "Could not closeSoftwareInputPanel");
#endif // Q_OS_SYMBIAN

    //Switch back to portrait
    screen->setProperty("allowedOrientations", SDeclarativeScreen::Portrait);

    QGraphicsObject *defocus = m_view->rootObject()->findChild<QGraphicsObject*>("defocus");
    QVERIFY(defocus);
    defocus->setFocus(Qt::OtherFocusReason);

    QApplication::processEvents();
}

void tst_SDeclarativeInputContext::visible()
{
    QGraphicsObject *textArea = m_view->rootObject()->findChild<QGraphicsObject*>("textArea");
    QGraphicsObject *textField = m_view->rootObject()->findChild<QGraphicsObject*>("textField");
    QGraphicsObject *defocus = m_view->rootObject()->findChild<QGraphicsObject*>("defocus");
    QGraphicsObject *closeIC = m_view->rootObject()->findChild<QGraphicsObject*>("closeIC");
    QVERIFY(textArea);
    QVERIFY(textField);
    QVERIFY(defocus);
    QVERIFY(closeIC);

    QTest::qWait(500);
    QSignalSpy visibleChangedSpy(m_inputContext, SIGNAL(visibleChanged()));

    //Make sure that input panel is not reguested on focus gain
    textField->setFocus(Qt::OtherFocusReason);
    textArea->setFocus(Qt::OtherFocusReason);
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 0);
    QCOMPARE(m_inputContext->property("visible").toBool(), false);

    // Request input panel for textArea
    QVERIFY2(QMetaObject::invokeMethod(textArea, "openSoftwareInputPanel"), "Could not openSoftwareInputPanel");
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 1);
    QCOMPARE(m_inputContext->property("visible").toBool(), true);

    // Focus out closes input panel
    defocus->setFocus(Qt::OtherFocusReason);
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 2);
    QCOMPARE(m_inputContext->property("visible").toBool(), false);

    // Request input panel for textField
    textField->setFocus(Qt::OtherFocusReason);
    QVERIFY2(QMetaObject::invokeMethod(textField, "openSoftwareInputPanel"), "Could not openSoftwareInputPanel");
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 3);
    QCOMPARE(m_inputContext->property("visible").toBool(), true);

    // Focus out closes input panel
    defocus->setFocus(Qt::OtherFocusReason);
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 4);
    QCOMPARE(m_inputContext->property("visible").toBool(), false);

    // Request input panel for textArea
    textArea->setFocus(Qt::OtherFocusReason);
    QVERIFY2(QMetaObject::invokeMethod(textArea, "openSoftwareInputPanel"), "Could not openSoftwareInputPanel");
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 5);
    QCOMPARE(m_inputContext->property("visible").toBool(), true);

    // Request input panel close
    QVERIFY2(QMetaObject::invokeMethod(textArea, "closeSoftwareInputPanel"), "Could not closeSoftwareInputPanel");
    QApplication::processEvents();
    QTRY_COMPARE(visibleChangedSpy.count(), 6);

    QCOMPARE(m_inputContext->property("visible").toBool(), false);
}

QTEST_MAIN(tst_SDeclarativeInputContext)

#include "tst_declarativeinputcontext.moc"
