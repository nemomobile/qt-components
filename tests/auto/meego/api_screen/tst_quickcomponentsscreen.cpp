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
#include <QtTest/QSignalSpy>
#include <QDeclarativeEngine>
#include <qdeclarativecontext.h>
#include <qdeclarativecomponent.h>
#include <qdeclarativeview.h>
#include <qdeclarativeitem.h>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsscreen : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void rotateToLandscape();
    void landscapeScreenWidth();
    void landscapeScreenHeight();
    void landscapeScreenDisplayWidth();
    void landscapeScreenDisplayHeight();

    void rotateToPortrait();
    void portraitScreenWidth();
    void portraitScreenHeight();
    void portraitScreenDisplayWidth();
    void portraitScreenDisplayHeight();

private:
    QObject *componentObject;
};

void tst_quickcomponentsscreen::init()
{
    // Initialize test
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsscreen.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsscreen::rotateToLandscape()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "rotateToLandscape", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCoreApplication::processEvents();

    QMetaObject::invokeMethod(componentObject, "getOrientation", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toString(), QString("Screen.Landscape"));
}

void tst_quickcomponentsscreen::landscapeScreenWidth()
{
    // Orientation changed to landscape in previous case
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentWidth", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toInt(), 854);
}

void tst_quickcomponentsscreen::landscapeScreenHeight()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentHeight", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toInt(), 480);
}

void tst_quickcomponentsscreen::landscapeScreenDisplayWidth()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentDisplayWidth", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toInt(), 854);
}

void tst_quickcomponentsscreen::landscapeScreenDisplayHeight()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentDisplayHeight", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toInt(), 480);
}

void tst_quickcomponentsscreen::rotateToPortrait()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "rotateToPortrait", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCoreApplication::processEvents();

    QMetaObject::invokeMethod(componentObject, "getOrientation", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toString(), QString("Screen.Portrait"));
}

void tst_quickcomponentsscreen::portraitScreenWidth()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentWidth", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
#ifdef USE_DEPRECATED_SCREEN_WIDTH_HEIGHT
    QEXPECT_FAIL("", "screen.width not yet updated -- rotation doesn't have effect", Continue);
#endif
    QCOMPARE(returnValue.toInt(), 480);
}

void tst_quickcomponentsscreen::portraitScreenHeight()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentHeight", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
#ifdef USE_DEPRECATED_SCREEN_WIDTH_HEIGHT
    QEXPECT_FAIL("", "screen.height not yet updated -- rotation doesn't have effect", Continue);
#endif
    QCOMPARE(returnValue.toInt(), 854);
}

void tst_quickcomponentsscreen::portraitScreenDisplayWidth()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentDisplayWidth", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toInt(), 854);
}

void tst_quickcomponentsscreen::portraitScreenDisplayHeight()
{
    QVariant returnValue;
    QMetaObject::invokeMethod(componentObject, "currentDisplayHeight", Qt::DirectConnection, Q_RETURN_ARG(QVariant, returnValue));
    QCOMPARE(returnValue.toInt(), 480);
}


QTEST_MAIN(tst_quickcomponentsscreen)

#include "tst_quickcomponentsscreen.moc"
