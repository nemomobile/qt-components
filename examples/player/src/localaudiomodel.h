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
