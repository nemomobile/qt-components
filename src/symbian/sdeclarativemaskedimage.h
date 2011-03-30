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

#ifndef SDECLARATIVEMASKEDIMAGE_H
#define SDECLARATIVEMASKEDIMAGE_H

#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeMaskedImagePrivate;

class SDeclarativeMaskedImage : public QDeclarativeItem
{
    Q_OBJECT

public:
    Q_PROPERTY(QString imageName READ imageName WRITE setImageName)
    Q_PROPERTY(QString maskName READ maskName WRITE setMaskName)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin)
    Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin)
    Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin)
    Q_PROPERTY(bool tiled READ isTiled WRITE setTiled)
    Q_PROPERTY(QPoint offset READ offset WRITE setOffset)

public:
    explicit SDeclarativeMaskedImage(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeMaskedImage();

    QString imageName() const;
    void setImageName(const QString &name);

    QString maskName() const;
    void setMaskName(const QString &name);

    QPoint offset() const;
    void setOffset(const QPoint &offset);

    bool isTiled() const;
    void setTiled(bool tiled);

    int topMargin() const;
    void setTopMargin(int margin);

    int bottomMargin() const;
    void setBottomMargin(int margin);

    int leftMargin() const;
    void setLeftMargin(int margin);

    int rightMargin() const;
    void setRightMargin(int margin);

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
    QScopedPointer<SDeclarativeMaskedImagePrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeMaskedImage)
    Q_DECLARE_PRIVATE(SDeclarativeMaskedImage)
};

QML_DECLARE_TYPE(SDeclarativeMaskedImage)

#endif // SDECLARATIVEMASKEDIMAGE_H
