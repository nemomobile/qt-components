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

#ifndef MDECLARATIVEMASKEDITEM_H
#define MDECLARATIVEMASKEDITEM_H

#include <QDeclarativeItem>
#include <QScopedPointer>

class QDeclarativeComponent;

class MDeclarativeMaskedItem : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QDeclarativeComponent *mask READ mask WRITE setMask NOTIFY maskChanged)
    Q_PROPERTY(QDeclarativeComponent *visualItem READ visualItem WRITE setVisualItem NOTIFY visualItemChanged)
    Q_PROPERTY(QPoint imageOffset READ imageOffset WRITE setImageOffset NOTIFY imageOffsetChanged)

public:
    MDeclarativeMaskedItem(QDeclarativeItem *parent=0);
    virtual ~MDeclarativeMaskedItem();

    QDeclarativeComponent *mask() const;
    void setMask(QDeclarativeComponent *mask);
    QDeclarativeComponent *visualItem() const;
    void setVisualItem(QDeclarativeComponent *item);
    QPoint imageOffset() const;
    void setImageOffset(const QPoint &imageOffset);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

signals:
    void maskChanged(QDeclarativeComponent *mask);
    void visualItemChanged(QDeclarativeComponent *visualItem);
    void imageOffsetChanged(const QPoint &imageOffset);

private:
    bool initializeMaskedBuffer();
    void paintMaskInBuffer();

    QDeclarativeComponent *m_maskComponent;
    QDeclarativeComponent *m_visualItemComponent;
    QScopedPointer<QDeclarativeItem> m_mask;
    QScopedPointer<QDeclarativeItem> m_visualItem;
    QImage m_buffer;
    QPoint m_imageOffset;

};

#endif //MDECLARATIVEMASKEDITEM_H
