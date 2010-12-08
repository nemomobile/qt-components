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

#ifndef MDECLARATIVEMASKEDIMAGE_H
#define MDECLARATIVEMASKEDIMAGE_H

#include "mdeclarativeprimitive.h"

class MScalableImage;

class MDeclarativeMaskedImage : public MDeclarativePrimitive
{
    Q_OBJECT

    Q_PROPERTY(int imageXOffset READ imageXOffset WRITE setImageXOffset);
    Q_PROPERTY(int imageYOffset READ imageYOffset WRITE setImageYOffset);
    Q_PROPERTY(QString imageProperty READ imageProperty WRITE setImageProperty);
    Q_PROPERTY(QString maskProperty READ maskProperty WRITE setMaskProperty);
    Q_PROPERTY(bool tiled READ isTiled WRITE setIsTiled);
    Q_PROPERTY(bool fullWidth READ isFullWidth WRITE setIsFullWidth);

public:
    MDeclarativeMaskedImage(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeMaskedImage();

    int imageXOffset() const;
    void setImageXOffset(int x);
    int imageYOffset() const;
    void setImageYOffset(int y);
    QString imageProperty() const;
    void setImageProperty(const QString &imageProperty);
    QString maskProperty() const;
    void setMaskProperty(const QString &maskProperty);
    bool isTiled() const;
    void setIsTiled(bool tiled);
    bool isFullWidth() const;
    void setIsFullWidth(bool isFullWidth);

protected:
    virtual void clearStyleData();
    virtual void fetchStyleData(const MStyle *style);
    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

    bool initializeMaskedBuffer();

    QPoint m_imageOffset;
    QString m_imageProperty;
    QString m_maskProperty;
    QScopedPointer<QImage> m_buffer;
    const MScalableImage *m_image;
    const MScalableImage *m_mask;
    bool m_isTiled;
    bool m_isFullWidth;
};

#endif //MDECLARATIVEMASKEDIMAGE_H
