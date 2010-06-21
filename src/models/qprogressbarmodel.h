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

#ifndef QPROGRESSBARMODEL_H
#define QPROGRESSBARMODEL_H

#include <QtCore/qobject.h>
#include <QtDeclarative/qdeclarative.h>

#include <kernel/common.h>

class QProgressBarModelPrivate;

class Q_COMPONENTS_EXPORT QProgressBarModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged);

public:
    QProgressBarModel(QObject *parent = 0);
    ~QProgressBarModel();

    int value() const;

Q_SIGNALS:
    void running();
    void paused();
    //void resumed();
    void canceled();
    void finished();
    void valueChanged(); //### ==running?

public Q_SLOTS:
    void start();
    void pause();
    void resume();
    void reset();
    void setValue(int value);

protected:
    QProgressBarModel(QProgressBarModelPrivate &dd, QObject *parent);
    QProgressBarModelPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QProgressBarModel)
    Q_DECLARE_PRIVATE(QProgressBarModel)
};

QML_DECLARE_TYPE(QProgressBarModel)

#endif // QPROGRESSBARMODEL_H
