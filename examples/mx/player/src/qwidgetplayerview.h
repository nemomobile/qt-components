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
