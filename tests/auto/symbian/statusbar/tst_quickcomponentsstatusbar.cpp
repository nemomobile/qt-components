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
#include <QTest>
#include <QUrl>
#include <QGraphicsObject>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QSignalSpy>

class tst_quickcomponentsstatusbar : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void signalWidth();
    void position();
    void height();
    void platformInverted();

private:
    QGraphicsObject *statusBar;
    QObject *priv;
    QScopedPointer<QDeclarativeView> view;
};

void tst_quickcomponentsstatusbar::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_quickcomponentsstatusbar.qml"));
    QVERIFY(view);
    view->show();
    view->activateWindow();
    QTest::qWaitForWindowShown(view.data());
    QVERIFY(view->rootObject());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));
    statusBar = view->rootObject()->findChild<QGraphicsObject*>("statusBar");
    QVERIFY(statusBar);
    priv = view->rootObject()->findChild<QObject*>("priv");
}

void tst_quickcomponentsstatusbar::signalWidth()
{
    // Only test if Default (NOT shared) status bar is used
    if(priv) {
        QVariant returnedValue = 0;
        int signalLevel = 0;
        QMetaObject::invokeMethod(priv, "signalWidthPercentage",
                                Q_RETURN_ARG(QVariant, returnedValue),
                                Q_ARG(QVariant, signalLevel));
        float percentage = returnedValue.toFloat();
        signalLevel = 50;
        QMetaObject::invokeMethod(priv, "signalWidthPercentage",
                                 Q_RETURN_ARG(QVariant, returnedValue),
                                Q_ARG(QVariant, signalLevel));
        QVERIFY(percentage < returnedValue.toFloat());
        percentage = returnedValue.toFloat();
        signalLevel = 100;
        QMetaObject::invokeMethod(priv, "signalWidthPercentage",
                                  Q_RETURN_ARG(QVariant, returnedValue),
                                  Q_ARG(QVariant, signalLevel));
        QVERIFY(percentage < returnedValue.toFloat());
    }
}

void tst_quickcomponentsstatusbar::position()
{
    QCOMPARE(statusBar->property("x").toInt(), 0);
    QCOMPARE(statusBar->property("y").toInt(), 0);
}

void tst_quickcomponentsstatusbar::height()
{
    QVERIFY(statusBar->property("implicitWidth").toInt() > 0);
    QVERIFY(statusBar->property("implicitHeight").toInt() > 0);
}

void tst_quickcomponentsstatusbar::platformInverted()
{
    QVERIFY(statusBar->property("platformInverted").isValid());
    QCOMPARE(statusBar->property("platformInverted").toBool(), false);
    statusBar->setProperty("platformInverted", QVariant(true));
    QCOMPARE(statusBar->property("platformInverted").toBool(), true);
}

QTEST_MAIN(tst_quickcomponentsstatusbar)

#include "tst_quickcomponentsstatusbar.moc"
