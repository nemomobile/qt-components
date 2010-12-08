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

#ifndef MDECLARATIVEBACKGROUND_H
#define MDECLARATIVEBACKGROUND_H

#include "mdeclarativeprimitive.h"
#include <MBackgroundTiles>

class MScalableImage;

class MDeclarativeBackground : public MDeclarativePrimitive
{
    Q_OBJECT

    Q_ENUMS(Position)

    Q_PROPERTY(Position tilePosition READ tilePosition WRITE setTilePosition)

public:
    MDeclarativeBackground(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeBackground();

    // XXX Ugly... Is there someway to export the M::Position type directly?
    enum Position {
        DefaultPosition = M::DefaultPosition,

        TopLeftPosition,
        TopCenterPosition,
        TopRightPosition,
        CenterLeftPosition,
        CenterPosition,
        CenterRightPosition,
        BottomLeftPosition,
        BottomCenterPosition,
        BottomRightPosition,

        HorizontalLeftPosition,
        HorizontalCenterPosition,
        HorizontalRightPosition,

        VerticalTopPosition,
        VerticalCenterPosition,
        VerticalBottomPosition
    };

    Position tilePosition() const;
    void setTilePosition(const Position position);

protected:
    virtual void clearStyleData();
    virtual void fetchStyleData(const MStyle *style);
    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    const MScalableImage *m_image;
    const MScalableImage *m_tiles;
    QColor m_color;
    Position m_tileposition;
};

#endif //MDECLARATIVEBACKGROUND_H
