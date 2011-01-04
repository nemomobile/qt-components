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

#ifndef SDECLARATIVEPROGRESSBARANIMATION_H
#define SDECLARATIVEPROGRESSBARANIMATION_H

#include <QDeclarativeItem>

class SDeclarativeProgressBarAnimationPrivate;

class SDeclarativeProgressBarAnimation : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QString animationIcon READ animationIcon WRITE setAnimationIcon)
    Q_PROPERTY(QString animationMask READ animationMask WRITE setAnimationMask)
    Q_PROPERTY(int animationOffset READ animationOffset WRITE setAnimationOffset)

public:

    explicit SDeclarativeProgressBarAnimation(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeProgressBarAnimation();

    QString animationIcon() const;
    void setAnimationIcon(const QString &name);

    QString animationMask() const;
    void setAnimationMask(const QString &name);

    int animationOffset() const;
    void setAnimationOffset(int offset);

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

protected:
    QScopedPointer<SDeclarativeProgressBarAnimationPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeProgressBarAnimation)
    Q_DECLARE_PRIVATE(SDeclarativeProgressBarAnimation)
};

QML_DECLARE_TYPE(SDeclarativeProgressBarAnimation)

#endif // SDECLARATIVEPROGRESSBARANIMATION_H
