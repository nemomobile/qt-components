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

#ifndef SFRAMEPOOL_H
#define SFRAMEPOOL_H

#include <QtGui/qpixmap.h>
#include "sdeclarativeframe.h"

QT_BEGIN_NAMESPACE
class QString;
class QSize;
QT_END_NAMESPACE

class SFramePool
{
public:
    static QPixmap get(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size);
    static void release(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size);

#ifdef FRAME_POOL_UNIT_TEST
    static int totalCount();
    static int count(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size);
#endif // FRAME_POOL_UNIT_TEST

private:
    static QPixmap loadFrame(const QString &filename, SDeclarativeFrame::FrameType type, const QSize &size);
    static QString createFileName(const QString &filename, const QString &suffix);
    SFramePool();
};

#endif // SFRAMEPOOL_H
