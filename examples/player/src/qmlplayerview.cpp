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

#include <QTime>
#include <QDebug>
#include <QDir>
#include <QEasingCurve>
#include "qmlplayerview.h"

QmlPlayerView::QmlPlayerView(QObject *parent)
    : AbstractPlayerView(parent),
      m_title(""),
      m_artist(""),
      m_totalTime(0),
      m_currentTime(0),
      m_volume(0),
      m_muted(false),
      m_shuffle(false),
      m_repeat(false),
      m_currentIndex(-1),
      m_state(Stopped),
      m_playlistModel(0),
      m_currentTheme(""),
      m_themesDir("qml/", QString(), QDir::Name, QDir::Dirs|QDir::NoDotAndDotDot),
      m_themesList(m_themesDir.entryList()),
      m_currentItem(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(scene);

    m_view->setWindowFlags(Qt::FramelessWindowHint);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setStyleSheet("background: transparent");
    m_view->setAttribute(Qt::WA_TranslucentBackground);
    m_view->setFrameStyle(0);

    m_engine.addImportPath("../../");
    m_engine.rootContext()->setContextProperty("view", this);

    m_fadeOut = new QPropertyAnimation(m_view, "windowOpacity", this);
    m_fadeIn  = new QPropertyAnimation(m_view, "windowOpacity", this);

    m_fadeOut->setStartValue(1.0);
    m_fadeOut->setEndValue(0.0);
    m_fadeOut->setDuration(300);

    m_fadeIn->setStartValue(0.0);
    m_fadeIn->setEndValue(1.0);
    m_fadeIn->setDuration(300);

    connect(this, SIGNAL(changeThemeClicked()), SLOT(changeTheme()));
    connect(m_fadeOut, SIGNAL(finished()), this, SLOT(showNewTheme()));

    setTheme("mx");
    m_view->show();
}

bool QmlPlayerView::setTheme(const QString &themeName)
{
    if (m_currentTheme == themeName)
        return true;

    QString theme(m_themesDir.absolutePath() + "/" + themeName + "/theme.qml");

    QFile themeFile(theme);

    if (!themeFile.exists()) {
        QMessageBox::warning(m_view,                             "Warning",
                             "Theme \""+ themeName + "\" corrupted" \
                             "or not found. Removed from the theme list");

        m_themesList.removeAll(themeName);
        return false;
    } else {
        m_currentTheme = themeName;

        m_themeComponent = new QDeclarativeComponent(&m_engine, QUrl(theme), this);

        // First setTheme is not animated
        if (!m_currentItem) {
            if (!m_themeComponent->isLoading()) {
                execute();
            } else {
                connect(m_themeComponent, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(execute()));
            }
        }

        return true;
    }
}

void QmlPlayerView::execute()
{
    disconnect(m_themeComponent, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(execute()));

    QObject *obj = m_themeComponent->create();

    if (obj) {
        if (QDeclarativeItem *newItem = qobject_cast<QDeclarativeItem *>(obj)) {
            if (m_currentItem) {
                m_view->scene()->removeItem(m_currentItem);
                delete m_currentItem;
            }

            m_view->scene()->addItem(newItem);
            m_view->resize(m_view->scene()->itemsBoundingRect().size().toSize());
            m_currentItem = newItem;
        }
    }
}

void QmlPlayerView::changeTheme()
{
    int index = m_themesList.indexOf(m_currentTheme);

    index = m_themesList[index] == m_themesList.last() ? 0 : index+1;

    if (setTheme(m_themesList[index]))
        m_fadeOut->start();
}

void QmlPlayerView::showNewTheme()
{
    execute();
    m_fadeIn->start();
}

void QmlPlayerView::setTitle(const QString &title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged();
}

void QmlPlayerView::setArtist(const QString &artist)
{
    if (m_artist == artist)
        return;

    m_artist = artist;
    emit artistChanged();
}

void QmlPlayerView::setTotalTime(qint64 time)
{
    if (m_totalTime == time)
        return;

    m_totalTime = time;
    emit totalTimeChanged();
}

void QmlPlayerView::setCurrentTime(qint64 time)
{
    if (m_currentTime == time)
        return;

    m_currentTime = time;
    emit currentTimeChanged();
}

void QmlPlayerView::setVolume(qreal volume)
{
    if (m_volume == volume)
        return;

    m_volume = volume;
    emit volumeChanged();
}

void QmlPlayerView::setMuted(bool state)
{
    if (m_muted == state)
        return;

    m_muted = state;
    emit mutedChanged();
}

void QmlPlayerView::setShuffle(bool state)
{
    if (m_shuffle == state)
        return;

    m_shuffle = state;
    emit shuffleChanged();
}

void QmlPlayerView::setRepeat(bool state)
{
    if (m_repeat == state)
        return;

    m_repeat = state;
    emit repeatChanged();
}

void QmlPlayerView::setCurrentIndex(int index)
{
    if (m_currentIndex == index)
        return;

    m_currentIndex = index;
    emit currentIndexChanged();
}

void QmlPlayerView::setState(State newState)
{
    if (m_state == newState)
        return;

    m_state = newState;
    emit stateChanged();
}

void QmlPlayerView::setPlaylistModel(AbstractPlaylistModel *model)
{
    if (m_playlistModel == model)
        return;

    m_playlistModel = model;
    emit playlistModelChanged();
}

QString QmlPlayerView::title()
{
    return m_title;
}

QString QmlPlayerView::artist()
{
    return m_artist;
}

qint64 QmlPlayerView::totalTime()
{
    return m_totalTime;
}

qint64 QmlPlayerView::currentTime()
{
    return m_currentTime;
}

bool QmlPlayerView::isMuted()
{
    return m_muted;
}

bool QmlPlayerView::isShuffle()
{
    return m_shuffle;
}

bool QmlPlayerView::isRepeat()
{
    return m_repeat;
}

qreal QmlPlayerView::volume()
{
    return m_volume;
}

QmlPlayerView::State QmlPlayerView::state()
{
    return m_state;
}

int QmlPlayerView::currentIndex()
{
    return m_currentIndex;
}

AbstractPlaylistModel *QmlPlayerView::playlistModel()
{
    return m_playlistModel;
}
