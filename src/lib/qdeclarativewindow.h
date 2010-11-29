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

#ifndef QDECLARATIVEWINDOW_H
#define QDECLARATIVEWINDOW_H

#if defined(QDECLARATIVEWINDOW_BUILD_LIB)
#  define QDECLARATIVEWINDOW_EXPORT Q_DECL_EXPORT
#else
#  define QDECLARATIVEWINDOW_EXPORT Q_DECL_IMPORT
#endif

#include <qwidget.h>
#include <qurl.h>
#include <qscopedpointer.h>

class QDeclarativeWindowPrivate;
class QDeclarativeEngine;
class QDeclarativeContext;
class QGraphicsObject;

class QDECLARATIVEWINDOW_EXPORT QDeclarativeWindow : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_ENUMS(Status)

public:
    QDeclarativeWindow();
    QDeclarativeWindow(const QUrl &source);
    ~QDeclarativeWindow();

    QUrl source() const;
    void setSource(const QUrl &url);

    QDeclarativeEngine *engine() const;
    QDeclarativeContext *rootContext() const;

    QGraphicsObject *rootObject() const;

    enum Status { Null, Ready, Loading, Error };
    Status status() const;

    QWidget *window() const;

Q_SIGNALS:
    void statusChanged(QDeclarativeWindow::Status);

protected:
    virtual void setRootObject(QObject *obj);

    QScopedPointer<QDeclarativeWindowPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_continueExecute())

    Q_DISABLE_COPY(QDeclarativeWindow)
    Q_DECLARE_PRIVATE(QDeclarativeWindow)
};

#endif
