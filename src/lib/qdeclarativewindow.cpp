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

#include "qdeclarativewindow.h"

#include <QApplication>
#include <QGLWidget>
#include <QtDeclarative>

#ifdef Q_COMPONENTS_MEEGO
#include <MComponentData>
#include <MDeviceProfile>
#endif

class QDeclarativeWindowPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeWindow)

public:
    QDeclarativeWindowPrivate(QDeclarativeWindow *qq);
    ~QDeclarativeWindowPrivate();

    void init();

    QDeclarativeWindow *q_ptr;
};

QDeclarativeWindowPrivate::QDeclarativeWindowPrivate(QDeclarativeWindow *qq)
    : q_ptr(qq)
{
}

QDeclarativeWindowPrivate::~QDeclarativeWindowPrivate()
{
}

void QDeclarativeWindowPrivate::init()
{
    Q_Q(QDeclarativeWindow);

#ifdef Q_COMPONENTS_MEEGO
    qApp->setProperty("NoMStyle", true);
    if (!MComponentData::instance()) {
        // This is a workaround because we can't use a default
        // constructor for MComponentData
        // ### this is only needed here for accessing MDeviceProfile::instance()->resolution()
        QByteArray argv0;
        if (!QCoreApplication::arguments().isEmpty())
            argv0 = QCoreApplication::arguments().first().toLocal8Bit();

        int argc = 1;
        char *argv[] = { argv0.data() };
        (void) new MComponentData(argc, argv);
    }

    q->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    q->setWindowFlags(Qt::Window
                      | Qt::CustomizeWindowHint
                      | Qt::FramelessWindowHint);

    q->resize(MDeviceProfile::instance()->resolution());
    q->setSceneRect(QRect(QPoint(), q->size()));
    q->setViewport(new QGLWidget);
#endif

    QObject::connect(q->engine(), SIGNAL(quit()), qApp, SLOT(quit()));
}

/*!
    Constructs a QDeclarativeWindow with the given \a parent.
 */
QDeclarativeWindow::QDeclarativeWindow(QWidget *parent) :
    QDeclarativeView(parent), d_ptr(new QDeclarativeWindowPrivate(this))
{
    Q_D(QDeclarativeWindow);
    d->init();
}

/*!
    Constructs a QDeclarativeWindow with the given QML \a source and \a parent.
 */
QDeclarativeWindow::QDeclarativeWindow(const QUrl &source, QWidget *parent) :
    QDeclarativeView(source, parent), d_ptr(new QDeclarativeWindowPrivate(this))
{
    Q_D(QDeclarativeWindow);
    d->init();
}

/*!
    Destroys the window.
 */
QDeclarativeWindow::~QDeclarativeWindow()
{
}

#include "moc_qdeclarativewindow.cpp"
