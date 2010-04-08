#include "qwidgetplayerview.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>

QWidgetPlayerView::QWidgetPlayerView(QObject *parent) : AbstractPlayerView(parent)
{
    m_playButton = new QPushButton("Play");
    m_stopButton = new QPushButton("Stop");
    m_rewButton = new QPushButton("Rew");
    m_fwdButton = new QPushButton("Fwd");
    m_volumeSlider = new QSlider(Qt::Vertical);
    m_volumeSlider->setRange(0, 70);

    connect(m_playButton, SIGNAL(clicked()), SIGNAL(playClicked()));
    connect(m_stopButton, SIGNAL(clicked()), SIGNAL(stopClicked()));
    connect(m_rewButton, SIGNAL(clicked()), SIGNAL(previousClicked()));
    connect(m_fwdButton, SIGNAL(clicked()), SIGNAL(nextClicked()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), SLOT(onVolumeSliderMoved(int)));

    m_title = new QLabel();
    m_artist = new QLabel();
    m_currentTime = new QLabel();
    m_totalTime = new QLabel();

    QVBoxLayout *externalVBox = new QVBoxLayout(&m_frame);
    QHBoxLayout *topHBox = new QHBoxLayout;
    QHBoxLayout *bottomHBox = new QHBoxLayout;
    QVBoxLayout *songVBox = new QVBoxLayout;

    externalVBox->addLayout(topHBox);
    externalVBox->addLayout(bottomHBox);

    topHBox->addWidget(m_volumeSlider);
    topHBox->addWidget(m_currentTime);
    topHBox->addWidget(m_totalTime);
    topHBox->addLayout(songVBox);

    bottomHBox->addWidget(m_rewButton);
    bottomHBox->addWidget(m_playButton);
    bottomHBox->addWidget(m_stopButton);
    bottomHBox->addWidget(m_fwdButton);

    songVBox->addWidget(m_title);
    songVBox->addWidget(m_artist);

    m_frame.resize(800, 480);
    m_frame.show();
}

void QWidgetPlayerView::setTitle(const QString &title)
{
    m_title->setText(title);
}

void QWidgetPlayerView::setArtist(const QString &artist)
{
    m_artist->setText(artist);
}

static QString formatTime(qint64 time)
{
    const int minutes = time / 60;
    const int seconds = time % 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QLatin1Char('0'));
}

void QWidgetPlayerView::setTotalTime(qint64 time)
{
    m_totalTime->setText(formatTime(time));
}

void QWidgetPlayerView::setCurrentTime(qint64 time)
{
    m_currentTime->setText(formatTime(time));
}

void QWidgetPlayerView::setVolume(qreal volume)
{
    int newVolume = volume * 70;

    if (m_volumeSlider->isSliderDown() || (qAbs(newVolume - m_volumeSlider->value()) <= 1))
        return;

    m_volumeSlider->setValue(volume * 70);
}

void QWidgetPlayerView::onVolumeSliderMoved(int value)
{
    emit volumeChanged(value / 70.0);
}
