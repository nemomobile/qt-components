#ifndef __ABSTRACTPLAYERVIEW_H__
#define __ABSTRACTPLAYERVIEW_H__

#include <QObject>
#include <QString>
#include <QAbstractListModel>

class AbstractPlaylistModel : public QAbstractListModel
{
    Q_OBJECT;
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
    AbstractPlaylistModel(QObject *parent = 0);

    enum Roles { Title = Qt::UserRole + 1,
                 Artist,
                 Duration };

signals:
    void rowCountChanged();
};

class AbstractPlayerView : public QObject
{
    Q_OBJECT;

public:
    AbstractPlayerView(QObject *parent = 0) : QObject(parent) {}

    enum State { Loading = 0,
                 Stopped,
                 Paused,
                 Playing,
                 Error };

    virtual void setTitle(const QString &title) = 0;
    virtual void setArtist(const QString &artist) = 0;
    virtual void setTotalTime(qint64 time) = 0;
    virtual void setCurrentTime(qint64 time) = 0;
    virtual void setVolume(qreal volume) = 0;
    virtual void setMuted(bool state) = 0;
    virtual void setShuffle(bool state) = 0;
    virtual void setRepeat(bool state) = 0;
    virtual void setCurrentIndex(int index) = 0;
    virtual void setState(State newState) = 0;
    virtual void setPlaylistModel(AbstractPlaylistModel *model) = 0;

signals:
    void playClicked();
    void pauseClicked();
    void stopClicked();
    void nextClicked();
    void previousClicked();
    void shuffleClicked();
    void muteClicked();
    void repeatClicked();
    void volumeChanged(qreal newVolume);
    void changeThemeClicked();
    void songSelected(int newSong);
};

inline AbstractPlaylistModel::AbstractPlaylistModel(QObject *parent)
  : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleMapping;
    roleMapping[AbstractPlaylistModel::Title] = "title";
    roleMapping[AbstractPlaylistModel::Artist] = "artist";
    roleMapping[AbstractPlaylistModel::Duration] = "duration";
    setRoleNames(roleMapping);

    connect(this, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            SIGNAL(rowCountChanged()));
    connect(this, SIGNAL(modelReset()), SIGNAL(rowCountChanged()));
}

#endif
