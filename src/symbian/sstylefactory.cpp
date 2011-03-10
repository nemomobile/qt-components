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

#define QT_NO_CAST_FROM_ASCII
#define QT_NO_CAST_TO_ASCII

#include "sstylefactory.h"
#include "sstyleengine.h"
#include "sdeclarativestyle.h"
#include "sdeclarativestyleinternal.h"
#include "sdeclarative.h"
#include "sdeclarativescreen.h"

#include <QObject>

class SStyleFactoryPrivate
{
public:
    SStyleFactoryPrivate() {}

    SStyleEngine *engine;
    SDeclarativeStyle *platformStyle;
    SDeclarativeStyleInternal *privateStyle;
};

SStyleFactory::SStyleFactory(SDeclarativeScreen *screen, QObject *parent)
    : QObject(parent),
      d_ptr(new SStyleFactoryPrivate())
{
    Q_D(SStyleFactory);
    d->engine = new SStyleEngine(screen, this);
    d->platformStyle = new SDeclarativeStyle(d->engine, this);
    d->privateStyle = new SDeclarativeStyleInternal(d->engine, this);
}

SStyleFactory::~SStyleFactory()
{
}

QObject *SStyleFactory::platformStyle() const
{
    Q_D(const SStyleFactory);
    return d->platformStyle;
}

QObject *SStyleFactory::privateStyle() const
{
    Q_D(const SStyleFactory);
    return d->privateStyle;
}

