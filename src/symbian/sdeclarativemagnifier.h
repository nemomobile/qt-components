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
#ifndef SDECLARATIVEMAGNIFIER_H
#define SDECLARATIVEMAGNIFIER_H

#include <QtDeclarative/qdeclarativeitem.h>
#include <QtCore/qrect.h>
#include <QtGui/qpixmap.h>

class SDeclarativeMagnifierPrivate;

class SDeclarativeMagnifier : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF sourceRect READ sourceRect WRITE setSourceRect NOTIFY sourceRectChanged)
    Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(QString overlayFileName READ overlayFileName WRITE setOverlayFileName NOTIFY overlayFileNameChanged)
    Q_PROPERTY(QString maskFileName READ maskFileName WRITE setMaskFileName NOTIFY maskFileNameChanged)

public:
    explicit SDeclarativeMagnifier(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeMagnifier();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *styleOption, QWidget *widget);

    void setSourceRect(const QRectF &rect);
    QRectF sourceRect() const;

    void setScaleFactor(qreal scaleFactor);
    qreal scaleFactor() const;

    void setOverlayFileName(const QString &overlayFileName);
    QString overlayFileName() const;

    void setMaskFileName(const QString &maskFileName);
    QString maskFileName() const;

signals:
    void sourceRectChanged();
    void scaleFactorChanged();
    void overlayFileNameChanged();
    void maskFileNameChanged();

protected:
    QScopedPointer<SDeclarativeMagnifierPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeMagnifier)
    Q_DECLARE_PRIVATE(SDeclarativeMagnifier)
};

#endif // SDECLARATIVEMAGNIFIER_H
