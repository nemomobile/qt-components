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

#ifndef SDECLARATIVE_H
#define SDECLARATIVE_H

#include <QtCore/qobject.h>
#include <QtDeclarative/qdeclarative.h>

class SDeclarativePrivate;

class SDeclarative : public QObject
{
    Q_OBJECT
    Q_PROPERTY(InteractionMode listInteractionMode READ listInteractionMode WRITE setListInteractionMode NOTIFY listInteractionModeChanged FINAL)
    Q_PROPERTY(QString currentTime READ currentTime NOTIFY currentTimeChanged)

    Q_ENUMS(InteractionMode ImageSize ScrollBarVisibility SourceSize EffectType Feedback)

public:
    SDeclarative(QObject *parent = 0);
    virtual ~SDeclarative();

    enum InteractionMode {
        TouchInteraction,
        KeyNavigation
    };

    // deprecated - Use platformStyle.graphicSizeXxxx instead
    enum ImageSize {
        Undefined = 0,
        Small,
        Medium,
        Large,
        ImagePortrait
    };

    // No duplicate prefix naming because enum values are only visible via qml
    enum ScrollBarVisibility {
        ScrollBarWhenNeeded = 0,
        ScrollBarWhenScrolling
    };

    enum SourceSize {
        UndefinedSourceDimension = -2
    };

    enum EffectType {
        FadeOut,
        FadeInFadeOut
    };

    enum Feedback {
        Basic,
        Sensitive,
        BasicButton,
        SensitiveButton,
        BasicKeypad,
        SensitiveKeypad,
        BasicSlider,
        SensitiveSlider,
        BasicItem,
        SensitiveItem,
        ItemScroll,
        ItemPick,
        ItemDrop,
        ItemMoveOver,
        BounceEffect,
        CheckBox,
        MultipleCheckBox,
        Editor,
        TextSelection,
        BlankSelection,
        LineSelection,
        EmptyLineSelection,
        PopUp,
        PopupOpen,
        PopupClose,
        Flick,
        StopFlick,
        MultiPointTouchActivate,
        RotateStep,
        LongPress,
        PositiveTacticon,
        NeutralTacticon,
        NegativeTacticon
    };

    static inline QString resolveIconFileName(const QString &iconName)
    {
        QString fileName = iconName;
        if (fileName.startsWith(QLatin1String("qtg")))
            fileName.prepend(QLatin1String(":/graphics/"));

        if (fileName.lastIndexOf(QLatin1Char('.')) == -1)
            fileName.append(QLatin1String(".svg"));

        return fileName;
    }

    InteractionMode listInteractionMode() const;
    void setListInteractionMode(InteractionMode mode);

    static QString currentTime();

Q_SIGNALS:
    void listInteractionModeChanged();
    void currentTimeChanged();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Q_DISABLE_COPY(SDeclarative)
    QScopedPointer<SDeclarativePrivate> d_ptr;
};

class SDialogStatus : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)

public:
    enum Status {
        Opening,
        Open,
        Closing,
        Closed
    };
};

class SPageOrientation : public QObject
{
    Q_OBJECT
    Q_ENUMS(PageOrientation)

public:
    enum PageOrientation {
        Automatic,
        LockPortrait,
        LockLandscape,
        LockPrevious,
        Manual
    };
};

class SPageStatus : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)

public:
    enum Status {
        Inactive,
        Activating,
        Active,
        Deactivating
    };
};

QML_DECLARE_TYPE(SDeclarative)

#endif // SDECLARATIVE_H
