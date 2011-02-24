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

#ifndef SDECLARATIVEMASKEDIMAGE_P_H
#define SDECLARATIVEMASKEDIMAGE_P_H

#include "sdeclarativeframe.h"
#include <QDeclarativeItem>
#include <QApplication>
#include <qglobal.h>

class SDeclarativeMaskedImagePrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeMaskedImage)

public:
    SDeclarativeMaskedImagePrivate(SDeclarativeMaskedImage *qq) :
        q_ptr(qq),
        maskType(SDeclarativeFrame::ThreePiecesHorizontal),
        offset(0, 0),
        tiled(false),
        pixmapsCreated(false),
        tileSize(0)
        {}

    void createImagePixmap();
    void createNonImagePixmap();
    void createMask();

    SDeclarativeMaskedImage *q_ptr;
    QString imageName;
    QString maskName;
    SDeclarativeFrame::FrameType maskType;
    QPoint offset;
    bool tiled;

    bool pixmapsCreated;
    QSize createdSize;
    QPixmap imagePixmap;
    QBitmap imageMask;
    int tileSize;
};

#endif // SDECLARATIVEMASKEDIMAGE_P_H
