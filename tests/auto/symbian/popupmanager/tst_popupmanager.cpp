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
#include "sdeclarativescreen.h"
#include <QTest>
#include <QSignalSpy>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QGraphicsObject>

class tst_popupmanager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void popupStackDepth();
    void duplicateOpen();

private:
    QScopedPointer<QDeclarativeView> view;
    QGraphicsObject *window;
};

void tst_popupmanager::initTestCase()
{
    view.reset(tst_quickcomponentstest::createDeclarativeView("tst_popupmanager.qml"));
    view->show();
    QVERIFY(view.data());
    QTest::qWaitForWindowShown(view.data());
    QTRY_COMPARE(QApplication::activeWindow(), static_cast<QWidget *>(view.data()));
    window = view->rootObject();
    QVERIFY(window);
}

void tst_popupmanager::popupStackDepth()
{
    window->setProperty("openDialogs", true);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 3);
    window->setProperty("dialogAmount", 2);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 2);
    window->setProperty("dialogAmount", 1);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 1);
    window->setProperty("dialogAmount", 0);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 0);
}

void tst_popupmanager::duplicateOpen()
{
    window->setProperty("duplicateOpen", true);
    window->setProperty("openDialogs", true);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 3);
    window->setProperty("dialogAmount", 2);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 2);
    window->setProperty("dialogAmount", 1);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 1);
    window->setProperty("dialogAmount", 0);
    QTRY_COMPARE(window->property("popupStackDepth").toInt(), 0);
}

QTEST_MAIN(tst_popupmanager)

#include "tst_popupmanager.moc"
