/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QRANGECONTROL_P_H
#define QRANGECONTROL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qobject_p.h"
#include "QtCore/qbasictimer.h"


QT_BEGIN_NAMESPACE

class QRangeControlPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QRangeControl)

    QRangeControlPrivate(QAbstractSlider *slider);
    QRangeControlPrivate(QGraphicsObject *graphicsObject);

    ~QRangeControlPrivate();

    void init();
    bool isSedated;
    bool signalsBlocked;

    QAbstractSlider *slider;
    QGraphicsObject *graphicsObject;

    void update()
    {
        if (isSedated)
            return;
        if (slider)
            slider->update();
        else if (graphicsObject)
            graphicsObject->update();
    }

    bool isRightToLeft() { return slider && slider->isRightToLeft(); }

    bool isEnabled()
    {
        return slider ? slider->isEnabled() :
                    graphicsObject ? graphicsObject->isEnabled() : false;
    }

    void setSteps(int single, int page);

    int minimum, maximum, pageStep, value, position, pressValue;
    int pos, minpos, maxpos;

    /**
     * Call effectiveSingleStep() when changing the slider value.
     */
    int singleStep;

    float offset_accumulated;
    uint tracking : 1;
    uint blocktracking :1;
    uint pressed : 1;
    uint invertedAppearance : 1;
    uint invertedControls : 1;
    uint wrapping : 1;
    uint stopMouseOverSlider : 1;
    Qt::Orientation orientation;

    QBasicTimer repeatActionTimer;
    int repeatActionTime;
    QRangeControl::SliderAction repeatAction;

    void wheelEventHandler(QEvent * e, int delta, Qt::KeyboardModifiers modifiers);

    void keyPressEventHandler(QKeyEvent *ev);
    void keyPressEventHandler(QGraphicsSceneEvent *ev);

#ifdef QT_KEYPAD_NAVIGATION
    bool hasEditFocus()
    {
        return slider && slider->hasEditFocus();
    }

    void setEditFocus(bool b)
    {
        if (slider)
            slider->setEditFocus(b);
    }

    int origValue;

    /**
     */
    bool isAutoRepeating;

    /**
     * When we're auto repeating, we multiply singleStep with this value to
     * get our effective step.
     */
    qreal repeatMultiplier;

    /**
     * The time of when the first auto repeating key press event occurs.
     */
    QTime firstRepeat;

#endif

    inline int effectiveSingleStep() const
    {
        return singleStep
#ifdef QT_KEYPAD_NAVIGATION
        * repeatMultiplier
#endif
        ;
    }

    inline int overflowSafeAdd(int add) const
    {
        int newValue = value + add;
        if (add > 0 && newValue < value)
            newValue = maximum;
        else if (add < 0 && newValue > value)
            newValue = minimum;
        return newValue;
    }

    inline void setAdjustedSliderPosition(int position)
    {
        Q_Q(QRangeControl);
        if (stopMouseOverSlider && (position > pressValue - 2 * pageStep)
            && (position < pressValue + 2 * pageStep)) {
            repeatAction = QRangeControl::SliderNoAction;
            q->setSliderPosition(pressValue);
            return;
        }
        q->triggerAction(repeatAction);
    }
};

QT_END_NAMESPACE

#endif // QRANGECONTROL_P_H
