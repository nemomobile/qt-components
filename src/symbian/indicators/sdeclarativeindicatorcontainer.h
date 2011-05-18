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

#ifndef SDECLARATIVEINDICATORCONTAINER_H
#define SDECLARATIVEINDICATORCONTAINER_H

#include "sdeclarativeimplicitsizeitem.h"

class SDeclarativeIndicatorContainerPrivate;

class SDeclarativeIndicatorContainer : public SDeclarativeImplicitSizeItem
{
    Q_OBJECT
    Q_PROPERTY(QColor indicatorColor READ indicatorColor WRITE setIndicatorColor NOTIFY indicatorColorChanged)
    Q_PROPERTY(int indicatorWidth READ indicatorWidth WRITE setIndicatorWidth NOTIFY indicatorSizeChanged)
    Q_PROPERTY(int indicatorHeight READ indicatorHeight WRITE setIndicatorHeight NOTIFY indicatorSizeChanged)
    Q_PROPERTY(int indicatorPadding READ indicatorPadding WRITE setIndicatorPadding NOTIFY indicatorPaddingChanged)
    Q_PROPERTY(int maxIndicatorCount READ maxIndicatorCount WRITE setMaxIndicatorCount NOTIFY maxIndicatorCountChanged)


public:
    SDeclarativeIndicatorContainer(QDeclarativeItem *parent = 0);
    ~SDeclarativeIndicatorContainer();

    QColor indicatorColor() const;
    void setIndicatorColor(const QColor &color);

    int indicatorWidth() const;
    void setIndicatorWidth(int width);

    int indicatorHeight() const;
    void setIndicatorHeight(int height);

    int indicatorPadding() const;
    void setIndicatorPadding(int padding);

    int maxIndicatorCount() const;
    void setMaxIndicatorCount(int maxCount);

public slots:
    void layoutChildren();

signals:
    void indicatorColorChanged(const QColor &color);
    void indicatorSizeChanged();
    void indicatorPaddingChanged(int padding);
    void maxIndicatorCountChanged(int maxCount);

protected:
    QScopedPointer<SDeclarativeIndicatorContainerPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeIndicatorContainer)
    Q_DECLARE_PRIVATE(SDeclarativeIndicatorContainer)

private slots:
    void doLayoutChildren();
};

QML_DECLARE_TYPE(SDeclarativeIndicatorContainer)

#endif // SDECLARATIVEINDICATORCONTAINER_H

