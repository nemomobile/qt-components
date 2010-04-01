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
