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

#ifndef SDECLARATIVESCREEN_H
#define SDECLARATIVESCREEN_H

#include <QDeclarativeItem>

class SDeclarativeScreenPrivate;

class SDeclarativeScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(QString orientationString READ orientationString NOTIFY orientationChanged FINAL)

    Q_PROPERTY(int width READ width NOTIFY sizeChanged)
    Q_PROPERTY(int height READ height NOTIFY sizeChanged)
    Q_PROPERTY(int rotation READ rotation NOTIFY orientationChanged FINAL)

    Q_PROPERTY(qreal ppi READ ppi)
    Q_PROPERTY(qreal ppmm READ ppmm)
    Q_PROPERTY(qreal unit READ unit NOTIFY displayChanged)
    Q_PROPERTY(QSizeF physicalSize READ physicalSize)

    Q_PROPERTY(bool minimized READ isMinimized WRITE setMinimized NOTIFY minimizedChanged FINAL)

    Q_ENUMS(Orientation)

public:
    explicit SDeclarativeScreen(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeScreen();

    enum Orientation {
        Automatic,
        Portrait,
        Landscape,
        PortraitInverted,
        LandscapeInverted
    };

    Orientation orientation() const;
    QString orientationString() const;
    void setOrientation(Orientation orientation);

    int rotation() const;

    int width() const;
    int height() const;

    bool isMinimized() const;
    void setMinimized(bool minimized);

    qreal ppi() const;
    qreal ppmm() const;
    qreal unit() const;
    QSizeF physicalSize() const;

Q_SIGNALS:
    void orientationChanged();
    void minimizedChanged();
    void sizeChanged();
    void displayChanged();

protected:
    QScopedPointer<SDeclarativeScreenPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_updateOrientationAngle())
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenSize(const QSize&))

    Q_DISABLE_COPY(SDeclarativeScreen)
    Q_DECLARE_PRIVATE(SDeclarativeScreen)
};

QML_DECLARE_TYPE(SDeclarativeScreen)

#endif // SDECLARATIVESCREEN_H
