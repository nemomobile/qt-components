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

#ifndef MDECLARATIVESCREEN_H
#define MDECLARATIVESCREEN_H

#include <qdeclarativeitem.h>
#include <qglobal.h>

class MDeclarativeScreenPrivate;

class MDeclarativeScreen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged FINAL)
    Q_PROPERTY(QString orientationString READ orientationString NOTIFY orientationChanged FINAL)
    Q_PROPERTY(bool orientationLocked READ isOrientationLocked WRITE setOrientationLocked NOTIFY orientationLockedChanged FINAL)
    Q_PROPERTY(bool covered READ isCovered NOTIFY coveredChanged FINAL)
    Q_PROPERTY(bool keyboardOpen READ isKeyboardOpen NOTIFY keyboardOpenChanged FINAL)

    Q_PROPERTY(int width READ width CONSTANT FINAL)
    Q_PROPERTY(int height READ height CONSTANT FINAL)
    Q_PROPERTY(int rotation READ rotation NOTIFY orientationChanged FINAL)

    Q_PROPERTY(bool minimized READ isMinimized WRITE setMinimized NOTIFY minimizedChanged FINAL)

    Q_PROPERTY(bool softwareInputPanelVisible READ softwareInputPanelVisible NOTIFY softwareInputPanelVisibleChanged FINAL)
    Q_PROPERTY(QRect softwareInputPanelRect READ softwareInputPanelRect NOTIFY softwareInputPanelVisibleChanged FINAL)

    Q_PROPERTY(QRectF microFocus READ microFocus NOTIFY microFocusChanged)

    Q_ENUMS(Orientation)

public:
    MDeclarativeScreen(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeScreen();

    enum Orientation {
        Portrait,
        Landscape,
        PortraitInverted,
        LandscapeInverted
    };

    void setOrientation(Orientation o);
    Orientation orientation() const;

    QString orientationString() const;

    int rotation() const;

    bool isOrientationLocked() const;
    void setOrientationLocked(bool locked);

    bool isCovered() const;
    bool isKeyboardOpen() const;

    int width() const;
    int height() const;

    bool softwareInputPanelVisible() const;
    QRect softwareInputPanelRect() const;

    QRectF microFocus() const;

    bool isMinimized() const;
    void setMinimized(bool minimized);

    // ### this is a workaround for QTBUG-15574
    Q_INVOKABLE void sendClicked(int x, int y, int position) const;

Q_SIGNALS:
    void orientationChanged();
    void orientationLockedChanged();
    void coveredChanged();
    void softwareInputPanelVisibleChanged();
    void minimizedChanged();
    void keyboardOpenChanged();
    void microFocusChanged();

private:
    Q_DISABLE_COPY(MDeclarativeScreen)
    Q_PRIVATE_SLOT(d, void _q_isCoveredChanged())
    Q_PRIVATE_SLOT(d, void _q_updateOrientationAngle())
    Q_PRIVATE_SLOT(d, void _q_setOrientationHelper())
    Q_PRIVATE_SLOT(d, void _q_checkMicroFocusHint())
    Q_PRIVATE_SLOT(d, void _q_sipChanged(const QRect &))

    friend class MDeclarativeScreenPrivate;
    MDeclarativeScreenPrivate *d;
};

QML_DECLARE_TYPE(MDeclarativeScreen)
#endif
