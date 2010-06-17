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

#ifndef __PLAYERCONTROLLER_H__
#define __PLAYERCONTROLLER_H__

#include "localaudiomodel.h"
#include "abstractplayerview.h"

#include <QObject>
#include <QPointer>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>

class FastPlaylistModel;
class PlayerController : public QObject
{
    Q_OBJECT;

public:
    PlayerController();
    virtual ~PlayerController();

    void setModel(LocalAudioModel *model);
    void setView(AbstractPlayerView *view);

    int currentSong() const;

signals:
    void songsAvailable();

public slots:
    void setCurrentSong(int idx);
    void play(int idx = -1);
    void pause();
    void stop();
    void next();
    void previous();
    void toggleShuffle();
    void toggleRepeat();
    void toggleMute();
    void setVolume(qreal volume);

protected:
    void createPlayer();
    void initializeView();
    void setCurrentSong_helper(int idx);
    int getNextSong();
    int getPreviousSong();
    void initializeShuffle();
    int nextShuffle();
    int previousShuffle();

    // MVC
    LocalAudioModel *m_model;
    QPointer<AbstractPlayerView> m_view;
    FastPlaylistModel *m_playlistModel;

    // State
    bool m_songsAvailable;
    int m_currentSong;
    bool m_shuffle;
    bool m_repeat;

    // Shuffle state
    QList<int> m_shuffledList;
    int m_shuffleIndex;

    // Phonon
    Phonon::MediaObject *m_mediaObject;
    Phonon::AudioOutput *m_audioOutput;
    Phonon::Path m_audioPath;

protected slots:
    void onModelReady();
    void onAboutToFinish();
    void onStateChanged(Phonon::State newState);
    void onTick(qint64 time);
    void onTotalTimeChanged(qint64 time);
};

class FastPlaylistModel : public AbstractPlaylistModel
{
    Q_OBJECT;

public:
    FastPlaylistModel(LocalAudioModel *source, QObject *parent = 0);

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

protected:
    LocalAudioModel *m_source;
    QList<int> m_duration;
};

#endif
