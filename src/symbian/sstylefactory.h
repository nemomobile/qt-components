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

#ifndef SSTYLEFACTORY_H
#define SSTYLEFACTORY_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qcolor.h>

class SStyleFactoryPrivate;
class SDeclarativeScreen;

class SStyleFactory : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject *platformStyle READ platformStyle CONSTANT FINAL)
    Q_PROPERTY(QObject *privateStyle READ privateStyle CONSTANT FINAL)

public:

    explicit SStyleFactory(SDeclarativeScreen *screen, QObject *parent = 0);
    ~SStyleFactory();

    QObject *platformStyle() const;
    QObject *privateStyle() const;

protected:
    QScopedPointer<SStyleFactoryPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SStyleFactory)
    Q_DECLARE_PRIVATE(SStyleFactory)
};

#endif // SSTYLEFACTORY_H
