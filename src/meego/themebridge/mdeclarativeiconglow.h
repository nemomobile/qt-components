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

#ifndef MDECLARATIVEICONGLOW_H
#define MDECLARATIVEICONGLOW_H

#include <QDeclarativeItem>

class MDeclarativeIconGlow : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged);
    Q_PROPERTY(QSize sourceSize READ sourceSize WRITE setSourceSize NOTIFY sourceSizeChanged);
    Q_PROPERTY(int glowRadius READ glowRadius WRITE setGlowRadius);
    Q_PROPERTY(QColor glowColor READ glowColor WRITE setGlowColor);

public:
    MDeclarativeIconGlow(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeIconGlow();

    QUrl source() const;
    void setSource(const QUrl &source);

    QSize sourceSize() const;
    void setSourceSize(const QSize &sourceSize);

    int glowRadius() const;
    void setGlowRadius(int glowRadius);

    QColor glowColor() const;
    void setGlowColor(QColor glowColor);

Q_SIGNALS:
    void sourceChanged(QUrl source);
    void sourceSizeChanged(QSize size);

protected:
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    bool loadIcon();

    QUrl m_source;
    QSize m_sourceSize;
    int m_glowRadius;
    QColor m_glowColor;

    QImage m_icon;
};

#endif //MDECLARATIVEICONGLOW_H
