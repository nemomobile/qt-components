/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MDECLARATIVESCREEN_H
#define MDECLARATIVESCREEN_H

#include <qglobal.h>
#include <QDesktopWidget>
#include <QPointer>
#include <QVector3D>
#include <QQuickItem>

#define MEEGOTOUCH_DOUBLETAP_INTERVAL 325

class QDeclarativeItem;
class MDeclarativeScreenPrivate;
class MWindowState;

#if defined(HAVE_XLIB)
typedef struct _XDisplay Display;
#endif

class MDeclarativeScreen : public QObject
{
    Q_OBJECT
public:
    enum Orientation {
        Default = 0,
        Portrait = 1,
        Landscape = 2,
        PortraitInverted = 4,
        LandscapeInverted = 8,
        All = 15
    };

    enum Direction {
        Clockwise = -1,
        NoDirection = 0,
        CounterClockwise = 1
    };
#ifdef ___arm___
    enum OrientationAngle {
        PortraitAngle = 270,
        LandscapeAngle = 0,
        PortraitInvertedAngle = 90,
        LandscapeInvertedAngle = 180
    };
#else
    enum OrientationAngle {
        PortraitAngle = 0,
        LandscapeAngle = 90,
        PortraitInvertedAngle = 180,
        LandscapeInvertedAngle = 270
    };
#endif

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

    Q_ENUMS(Orientation OrientationAngle Direction DisplayCategory Density)
    Q_FLAGS(Orientations)
    Q_DECLARE_FLAGS(Orientations, Orientation)

    Q_PROPERTY(Orientation currentOrientation READ currentOrientation NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(Orientations allowedOrientations READ allowedOrientations WRITE setAllowedOrientations NOTIFY allowedOrientationsChanged FINAL)
    Q_PROPERTY(QString orientationString READ orientationString NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(bool covered READ isCovered NOTIFY coveredChanged FINAL)
    Q_PROPERTY(bool keyboardOpen READ isKeyboardOpen NOTIFY keyboardOpenChanged FINAL)

    Q_PROPERTY(int width READ width NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height NOTIFY heightChanged FINAL)
    Q_PROPERTY(int displayWidth READ displayWidth NOTIFY displayChanged)
    Q_PROPERTY(int displayHeight READ displayHeight NOTIFY displayChanged)

    Q_PROPERTY(int rotation READ rotation NOTIFY currentOrientationChanged FINAL)
    Q_PROPERTY(Direction rotationDirection READ rotationDirection NOTIFY rotationDirectionChanged FINAL)
    Q_PROPERTY(bool minimized READ isMinimized WRITE setMinimized NOTIFY minimizedChanged FINAL)
    Q_PROPERTY(bool allowSwipe READ allowSwipe WRITE setAllowSwipe NOTIFY allowSwipeChanged FINAL )
    Q_PROPERTY(bool isPortrait READ isPortrait NOTIFY currentOrientationChanged FINAL )

    Q_PROPERTY(MWindowState * windowState READ windowState CONSTANT FINAL)

    Q_PROPERTY(qreal dpi READ dpi NOTIFY displayChanged FINAL)
    Q_PROPERTY(DisplayCategory displayCategory READ displayCategory NOTIFY displayChanged FINAL) // Small, Normal, Large, ExtraLarge
    Q_PROPERTY(Density density READ density NOTIFY displayChanged FINAL) // Low, Medium, High, ExtraHigh

    //this one is the orientation corrected screen resolution. So this values changes depending on orientation
    Q_PROPERTY(int platformWidth READ platformWidth NOTIFY platformWidthChanged FINAL)
    Q_PROPERTY(int platformHeight READ platformHeight NOTIFY platformHeightChanged FINAL)

    Q_PROPERTY(bool isDisplayLandscape READ isDisplayLandscape NOTIFY physicalDisplayChanged FINAL)

    Q_PROPERTY(int frameBufferRotation READ frameBufferRotation CONSTANT FINAL)

public:
    static MDeclarativeScreen* instance();
    virtual ~MDeclarativeScreen();

    Q_INVOKABLE void updatePlatformStatusBarRect(QQuickItem * statusBar);

    Orientation currentOrientation() const;

    Orientations allowedOrientations() const;

    QString orientationString() const;

    int rotation() const;
    Direction rotationDirection() const;

    bool isCovered() const;
    bool isKeyboardOpen() const;

    int width() const;
    int height() const;

    int platformWidth() const;
    int platformHeight() const;

    int displayWidth() const;
    int displayHeight() const;

    bool isMinimized() const;
    void setMinimized(bool minimized);
    bool allowSwipe() const;
    void setAllowSwipe(bool enabled);
    bool isPortrait() const;

    bool isDisplayLandscape() const;
    int frameBufferRotation() const;

    int dpi() const;
    DisplayCategory displayCategory() const;
    Density density() const;

    MWindowState * windowState() const;

#if defined(HAVE_XLIB)
    Display* display(QDeclarativeItem* item = 0) const;
#endif

    Orientations platformPhysicalDisplayOrientation() const;

    virtual bool eventFilter(QObject *, QEvent *);
public Q_SLOTS:
    void setAllowedOrientations(Orientations orientation);

Q_SIGNALS:
    void currentOrientationChanged();
    void allowedOrientationsChanged();
    void rotationDirectionChanged();
    void coveredChanged();
    void minimizedChanged();
    void keyboardOpenChanged();
    void displayChanged();
    void widthChanged();
    void heightChanged();
    void allowSwipeChanged();
    void isPortraitChanged();
    void platformWidthChanged();
    void platformHeightChanged();
    void physicalDisplayChanged();

private:
    MDeclarativeScreen(QObject *parent = 0);
    Q_DISABLE_COPY(MDeclarativeScreen)
    Q_PRIVATE_SLOT(d, void _q_isCoveredChanged())
    Q_PRIVATE_SLOT(d, void _q_updateOrientationAngle())
    Q_PRIVATE_SLOT(d, void _q_updateIsTvConnected())
    Q_PRIVATE_SLOT(d, void _q_windowAnimationChanged())

    void setOrientation(Orientation o);
    Orientation physicalOrientation() const;

    friend class MDeclarativeScreenPrivate;
    MDeclarativeScreenPrivate *d;
};

QML_DECLARE_TYPE(MDeclarativeScreen)
#endif
