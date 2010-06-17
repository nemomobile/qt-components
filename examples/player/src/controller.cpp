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

#include "controller.h"

PlayerController::PlayerController()
    : QObject(), m_model(0), m_playlistModel(0), m_songsAvailable(false),
      m_currentSong(-1), m_shuffle(false), m_repeat(false), m_shuffleIndex(-1),
      m_mediaObject(0), m_audioOutput(0)
{
    createPlayer();
}

PlayerController::~PlayerController()
{

}

void PlayerController::setModel(LocalAudioModel *model)
{
    if (model == m_model)
        return;

    if (m_model != NULL) {
        m_mediaObject->clear();
        m_songsAvailable = false;
        setCurrentSong_helper(-1);
        m_shuffledList.clear();
        m_shuffleIndex = -1;
        delete m_playlistModel;
        m_playlistModel = 0;
    }

    m_model = model;

    if (m_model != NULL) {
        if (m_model->isReady()) {
            onModelReady();
        } else {
            connect(m_model, SIGNAL(ready()), SLOT(onModelReady()));
        }
    }

    initializeView();
}

void PlayerController::setView(AbstractPlayerView *view)
{
    if (m_view == view)
        return;

    if (m_view) {
        m_view->disconnect(this);
    }

    m_view = view;

    if (m_view) {
        connect(m_view, SIGNAL(playClicked()), SLOT(play()));
        connect(m_view, SIGNAL(pauseClicked()), SLOT(pause()));
        connect(m_view, SIGNAL(stopClicked()), SLOT(stop()));
        connect(m_view, SIGNAL(nextClicked()), SLOT(next()));
        connect(m_view, SIGNAL(previousClicked()), SLOT(previous()));
        connect(m_view, SIGNAL(shuffleClicked()), SLOT(toggleShuffle()));
        connect(m_view, SIGNAL(muteClicked()), SLOT(toggleMute()));
        connect(m_view, SIGNAL(repeatClicked()), SLOT(toggleRepeat()));
        connect(m_view, SIGNAL(volumeChanged(qreal)), SLOT(setVolume(qreal)));
        connect(m_view, SIGNAL(songSelected(int)), SLOT(play(int)));
    }

    initializeView();
}

void PlayerController::initializeView()
{
    if (!m_view)
        return;

    // Initialize data from model
    if (m_songsAvailable) {
        m_view->setTitle(m_model->titleAt(m_currentSong));
        m_view->setArtist(m_model->artistAt(m_currentSong));
    } else {
        m_view->setTitle("");
        m_view->setArtist("");
    }

    // Initialize data from controller
    onTotalTimeChanged(m_mediaObject->totalTime());
    onTick(m_mediaObject->currentTime());
    onStateChanged(m_mediaObject->state());
    m_view->setVolume((m_audioOutput->volumeDecibel() + 35) / 35);
    m_view->setMuted(m_audioOutput->isMuted());
    m_view->setShuffle(m_shuffle);
    m_view->setRepeat(m_repeat);
    m_view->setPlaylistModel(m_playlistModel);
}

int PlayerController::currentSong() const
{
    return m_currentSong;
}

void PlayerController::setCurrentSong(int idx)
{
    if (!m_songsAvailable)
        return;

    if (m_currentSong == idx)
        return;

    if ((idx < 0) || (idx >= m_model->songCount())) {
        qWarning() << "PlayerController::setCurrentSong(): index out of bounds";
        return;
    }

    bool wasPlaying = (m_mediaObject->state() == Phonon::PlayingState);

    setCurrentSong_helper(idx);
    m_mediaObject->setCurrentSource(m_model->pathAt(idx));

    if (wasPlaying)
        m_mediaObject->play();
}

void PlayerController::setCurrentSong_helper(int idx)
{
    m_currentSong = idx;
    if (m_view) {
        if (idx < 0) {
            m_view->setTitle("");
            m_view->setArtist("");
            m_view->setCurrentIndex(-1);
        } else {
            m_view->setTitle(m_model->titleAt(m_currentSong));
            m_view->setArtist(m_model->artistAt(m_currentSong));
            m_view->setCurrentIndex(m_currentSong);
        }
        onTick(0);
    }
}

void PlayerController::play(int idx)
{
    if ((!m_songsAvailable) || (m_model->songCount() == 0))
        return;

    if (idx >= 0)
        setCurrentSong(idx);

    m_mediaObject->play();
}

void PlayerController::pause()
{
    m_mediaObject->pause();
}

void PlayerController::stop()
{
    m_mediaObject->stop();
    onTick(0);
}

void PlayerController::next()
{
    if (!m_songsAvailable)
        return;

    const int nextSong = getNextSong();
    if (nextSong >= 0)
        setCurrentSong(nextSong);
}

void PlayerController::previous()
{
    if (!m_songsAvailable)
        return;

    const int prevSong = getPreviousSong();
    if (prevSong >= 0)
        setCurrentSong(prevSong);
}

void PlayerController::toggleShuffle()
{
    if (m_shuffle) {
        m_shuffle = false;
        m_shuffledList.clear();
        m_shuffleIndex = -1;
    } else {
        m_shuffle = true;
        if (m_songsAvailable) {
            setCurrentSong(nextShuffle());
        }
    }

    if (m_view)
        m_view->setShuffle(m_shuffle);
}

void PlayerController::toggleRepeat()
{
    m_repeat = !m_repeat;

    if (m_view)
        m_view->setRepeat(m_repeat);
}

void PlayerController::toggleMute()
{
    bool mute = !m_audioOutput->isMuted();
    m_audioOutput->setMuted(mute);
    if (m_view)
        m_view->setMuted(mute);
}

void PlayerController::setVolume(qreal volume)
{
    qreal decibelVolume = (volume - 1) * 35;

    if (decibelVolume == m_audioOutput->volumeDecibel())
        return;

    if (volume == 0)
        m_audioOutput->setVolume(0);
    else
        m_audioOutput->setVolumeDecibel(decibelVolume);

    if (m_view)
        m_view->setVolume(volume);
}

void PlayerController::createPlayer()
{
    m_mediaObject = new Phonon::MediaObject(this);
    m_audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    m_audioPath = Phonon::createPath(m_mediaObject, m_audioOutput);

    m_mediaObject->setTickInterval(1000);

    connect(m_mediaObject, SIGNAL(aboutToFinish()),
            SLOT(onAboutToFinish()));
    connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
            SLOT(onStateChanged(Phonon::State)));
    connect(m_mediaObject, SIGNAL(tick(qint64)),
            SLOT(onTick(qint64)));
    connect(m_mediaObject, SIGNAL(totalTimeChanged(qint64)),
            SLOT(onTotalTimeChanged(qint64)));
}

int PlayerController::getNextSong()
{
    int currentIndex = m_shuffle ? m_shuffleIndex : m_currentSong;

    // No more songs ?
    if ((m_repeat == false) && (currentIndex == m_model->songCount() - 1))
        return -1;

    if (m_shuffle)
        return nextShuffle();
    else
        return (m_currentSong + 1) % m_model->songCount();
}

int PlayerController::getPreviousSong()
{
    int currentIndex = m_shuffle ? m_shuffleIndex : m_currentSong;

    if (currentIndex == 0)
        return -1;

    return m_shuffle ? previousShuffle() : m_currentSong - 1;
}

void PlayerController::initializeShuffle()
{
    qsrand(time(0));

    m_shuffledList.clear();

    for (int i = 0; i < m_model->songCount(); ++i)
        m_shuffledList << i;

    for (int i = m_model->songCount() - 1; i > 0; --i) {
        int chosenOne = qrand() % (i + 1);
        m_shuffledList.move(chosenOne, i);
    }

    m_shuffleIndex = 0;
}

int PlayerController::nextShuffle()
{
    m_shuffleIndex = (m_shuffleIndex + 1) % m_model->songCount();

    if (m_shuffleIndex == 0)
        initializeShuffle();

    return m_shuffledList.at(m_shuffleIndex);
}

int PlayerController::previousShuffle()
{
    Q_ASSERT(m_shuffleIndex > 0);

    --m_shuffleIndex;
    return m_shuffledList.at(m_shuffleIndex);
}

void PlayerController::onModelReady()
{
    disconnect(m_model, SIGNAL(ready()), this, SLOT(onModelReady()));
    if (m_model->songCount()) {
        m_songsAvailable = true;
        setCurrentSong(getNextSong());
        Q_ASSERT(m_playlistModel == 0);
        m_playlistModel = new FastPlaylistModel(m_model);
        initializeView();
        qWarning() << "Controller has songs available!";
        emit songsAvailable();
    }
}

void PlayerController::onAboutToFinish()
{
    int nextSong = getNextSong();

    if (nextSong >= 0) {
        m_mediaObject->enqueue(m_model->pathAt(nextSong));
        setCurrentSong_helper(nextSong);
    }
}

void PlayerController::onStateChanged(Phonon::State newState)
{
    switch (newState) {
    case Phonon::LoadingState:
        qWarning() << "Loading ";
        if (m_view)
            m_view->setState(AbstractPlayerView::Loading);
        break;
    case Phonon::StoppedState:
        qWarning() << "Stopped";
        if (m_view)
            m_view->setState(AbstractPlayerView::Stopped);
        break;
    case Phonon::PlayingState:
        qWarning() << "Playing";
        if (m_view)
            m_view->setState(AbstractPlayerView::Playing);
        break;
    case Phonon::BufferingState:
        qWarning() << "Buffering";
        if (m_view)
            m_view->setState(AbstractPlayerView::Loading);
        break;
    case Phonon::PausedState:
        if (m_view)
            m_view->setState(AbstractPlayerView::Paused);
        qWarning() << "Paused";
        break;
    case Phonon::ErrorState:
        if (m_view)
            m_view->setState(AbstractPlayerView::Error);

        if (m_mediaObject->errorType() == Phonon::NormalError) {
            qWarning() << "Normal error: " << m_mediaObject->errorString();
        } else if (m_mediaObject->errorType() == Phonon::FatalError) {
            Q_ASSERT(m_mediaObject->errorType() == Phonon::FatalError);
            qWarning() << "FATAL error: " << m_mediaObject->errorString();
        } else {
            qWarning() << "Strange error: " << m_mediaObject->errorString();
        }
        break;
    }
}

void PlayerController::onTick(qint64 time)
{
    if (m_view)
        m_view->setCurrentTime(qMax(0, int(time / 1000)));
}

void PlayerController::onTotalTimeChanged(qint64 time)
{
    if (m_view)
        m_view->setTotalTime(qMax(0, int(time / 1000)));

    if (m_playlistModel) {
        QModelIndex index = m_playlistModel->index(m_currentSong);
        m_playlistModel->setData(index,
                                 int(time / 1000),
                                 AbstractPlaylistModel::Duration);
    }
}

FastPlaylistModel::FastPlaylistModel(LocalAudioModel *source, QObject *parent)
  : AbstractPlaylistModel(parent), m_source(source)
{
    if (m_source) {
        for (int i = rowCount(); i > 0; --i)
            m_duration << -1;
    }
}

QVariant FastPlaylistModel::data(const QModelIndex &index, int role) const
{
    if ((index.row() < 0) || (index.row() >= rowCount(index.parent())))
        return QVariant();

    switch (role) {
    case Title:
        return m_source->titleAt(index.row());
    case Artist:
        return m_source->artistAt(index.row());
    case Duration:
        return m_duration.at(index.row());
    default:
        return QVariant();
    }
}

bool FastPlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    const int row = index.row();

    if ((row < 0) || (row >= rowCount(index.parent())))
        return false;

    if (role != Duration)
        return false;

    int newDuration = (!value.isValid() || (value.toInt() < 0)) ? -1 : value.toInt();

    if (newDuration != m_duration.at(row)) {
        m_duration[row] = newDuration;
        emit dataChanged(index, index);
    }

    return true;
}

int FastPlaylistModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_source ? m_source->songCount() : 0;
}
