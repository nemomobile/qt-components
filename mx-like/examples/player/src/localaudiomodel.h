#ifndef __LOCALAUDIO_H__
#define __LOCALAUDIO_H__

#include <QObject>
#include <QStringList>
#include <Phonon/Global>
#include <Phonon/MediaObject>

class LocalAudioModel : public QObject
{
    Q_OBJECT;

public:
    LocalAudioModel(const QString &path);

    int songCount() const;
    QString pathAt(int i) const;
    QString artistAt(int i) const;
    QString titleAt(int i) const;

    bool isReady() const;

signals:
    void ready();

protected slots:
    void onStateChange(Phonon::State newState);

protected:
    struct SongInfo {
        QString path;
        QString artist;
        QString title;
    };

    void loadingComplete();

    QList<SongInfo> m_songs;

    QList<Phonon::MediaObject *> m_parsers;
    int m_loadedSongs;
    QSet<int> badSongs;

    bool m_ready;
};

#endif
