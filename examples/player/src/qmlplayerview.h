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

#ifndef __QMLPLAYERVIEW_H__
#define __QMLPLAYERVIEW_H__

#include <QtDeclarative>
#include <QString>
#include <QAbstractListModel>

#include "abstractplayerview.h"

class QmlPlayerView : public AbstractPlayerView
{
    Q_OBJECT;
    Q_PROPERTY(QString title READ title NOTIFY titleChanged);
    Q_PROPERTY(QString artist READ artist NOTIFY artistChanged);
    Q_PROPERTY(qint64 totalTime READ totalTime NOTIFY totalTimeChanged);
    Q_PROPERTY(qint64 currentTime READ currentTime NOTIFY currentTimeChanged);
    Q_PROPERTY(qreal volume READ volume NOTIFY volumeChanged);
    Q_PROPERTY(bool muted READ isMuted NOTIFY mutedChanged);
    Q_PROPERTY(bool shuffle READ isShuffle NOTIFY shuffleChanged);
    Q_PROPERTY(bool repeat READ isRepeat NOTIFY repeatChanged);
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(int state READ state NOTIFY stateChanged);
    Q_PROPERTY(QObject *playlistModel READ playlistModel NOTIFY playlistModelChanged);

public:
    QmlPlayerView(QObject *parent = 0);

    bool setTheme(const QString &themeName);

    // Implementation of pure virtual controller interface
    virtual void setTitle(const QString &title);
    virtual void setArtist(const QString &artist);
    virtual void setTotalTime(qint64 time);
    virtual void setCurrentTime(qint64 time);
    virtual void setVolume(qreal volume);
    virtual void setMuted(bool state);
    virtual void setShuffle(bool state);
    virtual void setRepeat(bool state);
    virtual void setCurrentIndex(int index);
    virtual void setState(State newState);
    virtual void setPlaylistModel(AbstractPlaylistModel *model);

    // Property access methods
    QString title();
    QString artist();
    qint64 totalTime();
    qint64 currentTime();
    qreal volume();
    bool isMuted();
    bool isShuffle();
    bool isRepeat();
    int currentIndex();
    State state();
    AbstractPlaylistModel *playlistModel();

signals:
    // Property notification signals
    void titleChanged();
    void artistChanged();
    void totalTimeChanged();
    void currentTimeChanged();
    void volumeChanged();
    void mutedChanged();
    void shuffleChanged();
    void repeatChanged();
    void currentIndexChanged();
    void stateChanged();
    void playlistModelChanged();

private slots:
    void changeTheme();
    void showNewTheme();
    void execute();

private:
    QString m_title;
    QString m_artist;
    qint64 m_totalTime;
    qint64 m_currentTime;
    qreal m_volume;
    bool m_muted;
    bool m_shuffle;
    bool m_repeat;
    int m_currentIndex;
    State m_state;
    AbstractPlaylistModel *m_playlistModel;
    QString m_currentTheme;
    QDir m_themesDir;
    QStringList m_themesList;

    QPropertyAnimation *m_fadeOut;
    QPropertyAnimation *m_fadeIn;

    QGraphicsView *m_view;
    QDeclarativeEngine m_engine;

    QDeclarativeComponent *m_themeComponent;
    QDeclarativeItem *m_currentItem;
};

QML_DECLARE_TYPE(QmlPlayerView)

#endif
