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

#include "sdeclarativenetworkindicator_p.h"
#include "sdeclarativenetworkindicator.h"

#include <QPainter>
#include <QPixmap>

// This is the fallback (desktop) implementation of
// SDeclarativeNetworkIndicatorPrivate that's always showing the default
// network icon. The Symbian specific implementation is in another file.

class SDeclarativeNetworkIndicatorPrivateImpl
{
public:
    SDeclarativeNetworkIndicatorPrivateImpl() {};
    ~SDeclarativeNetworkIndicatorPrivateImpl() {};

    QSize currentSize;
    QPixmap pixmap;
    QPixmap scaledPixmap;
};

SDeclarativeNetworkIndicatorPrivate::SDeclarativeNetworkIndicatorPrivate(
    SDeclarativeNetworkIndicator *qq) : offline(false), q_ptr(qq)
{
    impl = new SDeclarativeNetworkIndicatorPrivateImpl();
    impl->pixmap = QPixmap(":/graphics/qtg_graf_signal_icon.svg");
}

SDeclarativeNetworkIndicatorPrivate::~SDeclarativeNetworkIndicatorPrivate()
{
    delete impl;
}

void SDeclarativeNetworkIndicatorPrivate::reset()
{
    // Nothing to do.
}

QPixmap SDeclarativeNetworkIndicatorPrivate::pixmap()
{
    QSize newSize(q_ptr->width(), q_ptr->height());
    if (newSize != impl->currentSize) {
        impl->currentSize = newSize;
        impl->scaledPixmap = impl->pixmap.scaled(impl->currentSize,
            Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return impl->scaledPixmap;
}

