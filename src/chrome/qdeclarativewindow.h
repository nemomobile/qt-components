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

#include <qwidget.h>
#include <qurl.h>

class QDeclarativeWindowPrivate;
class QDeclarativeEngine;
class QDeclarativeContext;
class QGraphicsObject;

class QDeclarativeWindow : public QWidget
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

    QDeclarativeEngine *engine();
    QDeclarativeContext *rootContext();

    QGraphicsObject *rootObject() const;

    enum Status { Null, Ready, Loading, Error };
    Status status() const;

Q_SIGNALS:
    void statusChanged(QDeclarativeWindow::Status);

protected:
    virtual void setRootObject(QObject *obj);

private Q_SLOTS:
    void continueExecute();

private:
    friend class QDeclarativeWindowPrivate;
    QDeclarativeWindowPrivate *d;
};

#endif
