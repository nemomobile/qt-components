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

    Q_PROPERTY(int width READ width NOTIFY displayChanged FINAL)
    Q_PROPERTY(int height READ height NOTIFY displayChanged FINAL)
    Q_PROPERTY(qreal dpi READ dpi NOTIFY displayChanged FINAL)
    Q_PROPERTY(int rotation READ rotation NOTIFY orientationChanged FINAL)

    Q_ENUMS(Orientation)

public:
    explicit SDeclarativeScreen(QObject *parent = 0);
    virtual ~SDeclarativeScreen();

    enum Orientation {
        Automatic,
        Portrait,
        Landscape,
        PortraitInverted,
        LandscapeInverted
    };

    Orientation orientation() const;
    void setOrientation(Orientation orientation);

    int width() const;
    int height() const;
    qreal dpi() const;
    int rotation() const;

    Q_INVOKABLE void setDisplay(int width, int height, qreal dpi);

Q_SIGNALS:
    void orientationChanged();
    void displayChanged();
    void statusPaneChanged();
    // correct underscore naming for private signals is not possible yet due to QTBUG-17950
    void privateAboutToUpdateScreen();
    void privateScreenUpdated();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    QScopedPointer<SDeclarativeScreenPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_initView(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_updateScreenSize(const QSize &))
    Q_PRIVATE_SLOT(d_func(), void _q_desktopResized(int))
    Q_DISABLE_COPY(SDeclarativeScreen)
    Q_DECLARE_PRIVATE(SDeclarativeScreen)
    friend class SDeclarativeWindowDecorationPrivate;
};

QML_DECLARE_TYPE(SDeclarativeScreen)

#endif // SDECLARATIVESCREEN_H
