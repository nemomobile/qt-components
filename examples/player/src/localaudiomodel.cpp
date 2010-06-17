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

#include "localaudiomodel.h"

#include <QDir>

LocalAudioModel::LocalAudioModel(const QString &path)
  : QObject(), m_loadedSongs(0), m_ready(false)
{
    // Get the name of all files that match "*.mp3"
    QDir dir(path);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList() << "*.mp3");
    QStringList filenames = dir.entryList();

    for (int i = 0; i < filenames.count(); ++i) {
        // Initialize
        m_songs.append(SongInfo());
        m_songs[i].path = dir.filePath(filenames.at(i));

        // Create MediaObject to parse file
        Phonon::MediaObject *parser = new Phonon::MediaObject(this);
        m_parsers.append(parser);
        connect(parser, SIGNAL(stateChanged(Phonon::State, Phonon::State)),
                SLOT(onStateChange(Phonon::State)));
        parser->setCurrentSource(m_songs[i].path);

        // Create reverse mapping
        parser->setProperty("songIndex", i);
    }
}

void LocalAudioModel::onStateChange(Phonon::State newState)
{
    Phonon::MediaObject *parser = static_cast<Phonon::MediaObject *>(sender());
    const int idx = parser->property("songIndex").toInt();

    if (newState == Phonon::StoppedState) {
        // Loading was fine
        m_songs[idx].artist = parser->metaData(Phonon::ArtistMetaData).first();
        m_songs[idx].title = parser->metaData(Phonon::TitleMetaData).first();
        ++m_loadedSongs;
    } else if (newState == Phonon::ErrorState) {
        // Error loading, save index for this song
        badSongs << idx;
    } else {
        return;
    }

    // Delete parser
    m_parsers[idx] = 0;
    parser->deleteLater();

    if ((m_loadedSongs + badSongs.count()) == m_songs.count())
        loadingComplete();
}

void LocalAudioModel::loadingComplete()
{
    QList<int> badSongs_list = badSongs.toList();
    qSort(badSongs_list);
    for (int i = badSongs_list.count() - 1; i >= 0; --i) {
        m_songs.removeAt(badSongs_list.at(i));
    }

    badSongs.clear();
    m_loadedSongs = 0;
    m_ready = true;

    emit ready();
}

int LocalAudioModel::songCount() const
{
    return m_songs.count();
}

QString LocalAudioModel::pathAt(int i) const
{
    return m_songs.at(i).path;
}

QString LocalAudioModel::artistAt(int i) const
{
    return m_songs.at(i).artist;
}

QString LocalAudioModel::titleAt(int i) const
{
    return m_songs.at(i).title;
}

bool LocalAudioModel::isReady() const
{
    return m_ready;
}
