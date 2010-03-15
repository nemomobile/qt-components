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

#ifndef QRANGECONTROL_H
#define QRANGECONTROL_H

#include <QtCore/qobject.h>
#include <QtGui/qgraphicsitem.h>
#include <QtGui/qabstractslider.h>
#include <QtDeclarative/qdeclarative.h>

class QRangeControlPrivate;

class QRangeControl : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int minimumValue READ minimum WRITE setMinimum NOTIFY rangeChanged)
    Q_PROPERTY(int maximumValue READ maximum WRITE setMaximum NOTIFY rangeChanged)
    Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int pageStep READ pageStep WRITE setPageStep)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(int sliderPosition READ sliderPosition WRITE setSliderPosition NOTIFY sliderMoved)
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged) // ###
    Q_PROPERTY(int minimumPosition READ minimumPosition WRITE setMinimumPosition NOTIFY positionRangeChanged) // ###
    Q_PROPERTY(int maximumPosition READ maximumPosition WRITE setMaximumPosition NOTIFY positionRangeChanged) // ###
    Q_PROPERTY(bool tracking READ hasTracking WRITE setTracking)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(bool invertedAppearance READ invertedAppearance WRITE setInvertedAppearance)
    Q_PROPERTY(bool invertedControls READ invertedControls WRITE setInvertedControls)
    Q_PROPERTY(bool sliderDown READ isSliderDown WRITE setSliderDown DESIGNABLE false)
    Q_PROPERTY(bool wrapping READ wrapping WRITE setWrapping)

public:
    QRangeControl(QAbstractSlider *widget = 0);
    QRangeControl(QGraphicsObject *graphicsObject);
    virtual ~QRangeControl();

    void sedate();
    void awake();

    Qt::Orientation orientation() const;

    void setMinimum(int);
    int minimum() const;

    void setMaximum(int);
    int maximum() const;

    void setRange(int min, int max);
    bool hasNullRange() const;  // min == max

    int bound(int val) const;

    void setSingleStep(int);
    int singleStep() const;

    void setPageStep(int);
    int pageStep() const;

    void setTracking(bool enable);
    bool hasTracking() const;

    void setSliderDown(bool);
    bool isSliderDown() const;

    void setSliderPosition(int);
    int sliderPosition() const;

    void setMinimumPosition(int);
    int minimumPosition() const;

    void setMaximumPosition(int);
    int maximumPosition() const;

    void setPositionRange(int min, int max);

    void setPosition(int position);
    int position() const;

    void setPressValue(int);
    int pressValue() const;

    void setInvertedAppearance(bool);
    bool invertedAppearance() const;

    void setInvertedControls(bool);
    bool invertedControls() const;

    void setWrapping(bool);
    bool wrapping() const;

    // Control property linked to the QStyle::SH_Slider_StopMouseOverSlider style hint.
    void setStopMouseOverSlider(bool);
    bool stopMouseOverSlider() const;

    void startRepeatActionTimer(int ms);
    bool repeatActionTimerActive() const;

    enum SliderAction {
        SliderNoAction,
        SliderSingleStepAdd,
        SliderSingleStepSub,
        SliderPageStepAdd,
        SliderPageStepSub,
        SliderToMinimum,
        SliderToMaximum,
        SliderMove
    };

    int value() const;

#ifdef QT_KEYPAD_NAVIGATION
    void saveValue();
#endif

    void triggerAction(SliderAction action);

public Q_SLOTS:
    void setValue(int);
    void setOrientation(Qt::Orientation);

Q_SIGNALS:
    void valueChanged(int value);

    void sliderPressed();
    void sliderMoved(int position);
    void sliderReleased();

    void positionChanged(int position);

    void rangeChanged(int min, int max);
    void positionRangeChanged(int min, int max);

    void actionTriggered(int action);

public:
    void setRepeatAction(SliderAction action, int thresholdTime = 500, int repeatTime = 50);
    SliderAction repeatAction() const;

    enum SliderChange {
        SliderRangeChange,
        SliderOrientationChange,
        SliderStepsChange,
        SliderValueChange
    };
    virtual void sliderChange(SliderChange change);

    void keyPressEvent(QEvent *ev);
    void timerEvent(QTimerEvent *);
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QEvent *e);
#endif
    void changeEvent(QEvent *e);

protected:
    QRangeControl(QRangeControlPrivate &dd, QObject *parent);

private:
    Q_DISABLE_COPY(QRangeControl)
    Q_DECLARE_PRIVATE(QRangeControl)

};

QML_DECLARE_TYPE(QRangeControl);

#endif // QRANGECONTROL_H
