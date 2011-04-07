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

#ifndef SDECLARATIVESCREEN_H
#define SDECLARATIVESCREEN_H

#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeScreenPrivate;
QT_FORWARD_DECLARE_CLASS(QDeclarativeEngine)

class SDeclarativeScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ width NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height NOTIFY heightChanged FINAL)
    Q_PROPERTY(int displayWidth READ displayWidth NOTIFY displayChanged FINAL)
    Q_PROPERTY(int displayHeight READ displayHeight NOTIFY displayChanged FINAL)

    Q_PROPERTY(int rotation READ rotation NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged FINAL) // deprecated
    Q_PROPERTY(Orientation currentOrientation READ currentOrientation NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(Orientations allowedOrientations READ allowedOrientations WRITE setAllowedOrientations NOTIFY allowedOrientationsChanged FINAL)

    Q_PROPERTY(qreal dpi READ dpi NOTIFY displayChanged FINAL)
    Q_PROPERTY(DisplayCategory displayCategory READ displayCategory NOTIFY displayChanged FINAL)
    Q_PROPERTY(Density density READ density NOTIFY displayChanged FINAL)

    Q_ENUMS(Orientation DisplayCategory Density)
    Q_FLAGS(Orientations)

public:
    explicit SDeclarativeScreen(QDeclarativeEngine *engine, QObject *parent = 0);
    virtual ~SDeclarativeScreen();

    enum Orientation {
        Default = 0,
        Portrait = 1,
        Landscape = 2,
        PortraitInverted = 4,
        LandscapeInverted = 8,
        All = 15,
        Automatic = All // deprecated
    };

    enum DisplayCategory {
        Small,
        Normal,
        Large,
        ExtraLarge
    };

    enum Density {
        Low,
        Medium,
        High,
        ExtraHigh
   };

    Q_DECLARE_FLAGS(Orientations, Orientation)

    int width() const;
    int height() const;
    int displayWidth() const;
    int displayHeight() const;

    int rotation() const;
    Orientation orientation() const; //deprecated
    void setOrientation(Orientation orientation); //deprecated
    Orientation currentOrientation() const;
    Orientations allowedOrientations() const;
    void setAllowedOrientations(Orientations orientations);

    qreal dpi() const;
    DisplayCategory displayCategory() const;
    Density density() const;

    Q_INVOKABLE void privateSetDisplay(int width, int height, qreal dpi);

Q_SIGNALS:
    void widthChanged();
    void heightChanged();
    void orientationChanged(); // deprecated
    void currentOrientationChanged();
    void allowedOrientationsChanged();
    void displayChanged();
    void privateAboutToChangeOrientation();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    QScopedPointer<SDeclarativeScreenPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_initView(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenSize(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_desktopResized(int))
    Q_DISABLE_COPY(SDeclarativeScreen)
    Q_DECLARE_PRIVATE(SDeclarativeScreen)
};

QML_DECLARE_TYPE(SDeclarativeScreen)

Q_DECLARE_OPERATORS_FOR_FLAGS(SDeclarativeScreen::Orientations)

#endif // SDECLARATIVESCREEN_H
