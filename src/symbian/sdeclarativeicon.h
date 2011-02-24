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

#ifndef SDECLARATIVEICON_H
#define SDECLARATIVEICON_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeIconPrivate;

class SDeclarativeIcon : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged)

public:
    SDeclarativeIcon(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeIcon();

    QString iconName() const;
    void setIconName(const QString &name);

    QColor iconColor() const;
    void setIconColor(const QColor &color);

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

Q_SIGNALS:
    void iconNameChanged(const QString &name);
    void iconColorChanged(const QColor &color);

protected:
    QScopedPointer<SDeclarativeIconPrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeIcon)
    Q_DECLARE_PRIVATE(SDeclarativeIcon)
};

QML_DECLARE_TYPE(SDeclarativeIcon)

#endif // SDECLARATIVEICON_H
