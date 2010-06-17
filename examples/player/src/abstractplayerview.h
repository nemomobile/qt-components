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
