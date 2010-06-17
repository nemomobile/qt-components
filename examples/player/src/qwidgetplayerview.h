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

#ifndef __QWIDGETPLAYERVIEW_H__
#define __QWIDGETPLAYERVIEW_H__

#include "abstractplayerview.h"

#include <QLabel>

class QWidget;
class QPushButton;
class QSlider;

class QWidgetPlayerView : public AbstractPlayerView
{
    Q_OBJECT;

public:
    QWidgetPlayerView(QObject *parent = 0);

    virtual void setTitle(const QString &title);
    virtual void setArtist(const QString &artist);
    virtual void setTotalTime(qint64 time);
    virtual void setCurrentTime(qint64 time);
    virtual void setVolume(qreal volume);
    virtual void setMuted(bool) { };
    virtual void setShuffle(bool) { };
    virtual void setRepeat(bool) { };
    virtual void setCurrentIndex(int) { };
    virtual void setState(State) { };
    virtual void setPlaylistModel(AbstractPlaylistModel *) { };

protected slots:
    void onVolumeSliderMoved(int value);

protected:
    QWidget m_frame;
    QPushButton *m_playButton;
    QPushButton *m_stopButton;
    QPushButton *m_rewButton;
    QPushButton *m_fwdButton;
    QLabel *m_title;
    QLabel *m_artist;
    QLabel *m_currentTime;
    QLabel *m_totalTime;
    QSlider *m_volumeSlider;
};

#endif
