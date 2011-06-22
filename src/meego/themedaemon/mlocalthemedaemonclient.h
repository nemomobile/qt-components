/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MLOCALTHEMEDAEMONCLIENT_H
#define MLOCALTHEMEDAEMONCLIENT_H

#include <themedaemon/mabstractthemedaemonclient.h>

#include <QHash>
#include <QPixmap>
#include <QString>

class QDir;
/**
 * \brief Allows to request pixmaps from a local themedaemon server.
 *
 * The requested pixmaps are cached so that multiple requests of the
 * same pixmap can be handled fast.
 */
class MLocalThemeDaemonClient : public MAbstractThemeDaemonClient
{
    Q_OBJECT

public:
    /**
     * \param path   File path where the icons and images are located.
     *               If no file path is provided, the default path defined
     *               by the define THEME_DIR is used.
     * \param parent Parent object.
     */
    MLocalThemeDaemonClient(const QString &path = QString(), QObject *parent = 0);
    virtual ~MLocalThemeDaemonClient();

    /**
     * \see MAbstractThemeDaemonClient::requestPixmap()
     */
    virtual QPixmap requestPixmap(const QString &id, const QSize &requestedSize);

private:
    /**
     * Reads the image \a id from the available directories specified
     * by m_imageDirNodes.
     */
    QImage readImage(const QString &id) const;

    static QString findFileRecursively(const QDir& rootDir, const QString& name);

    void buildHash(const QDir& rootDir, const QStringList& nameFilter);

    /**
     * Cache entry that identifies a pixmap by a string-ID and size.
     */
    struct PixmapIdentifier
    {
        PixmapIdentifier();
        PixmapIdentifier(const QString &imageId, const QSize &size);
        QString imageId;
        QSize size;
        bool operator==(const PixmapIdentifier &other) const;
        bool operator!=(const PixmapIdentifier &other) const;
    };

    struct ImageDirNode
    {
        ImageDirNode(const QString &directory, const QStringList &suffixList);
        QString directory;
        QStringList suffixList;
    };

    QString m_path;
    QHash<PixmapIdentifier, QPixmap> m_pixmapCache;
    QList<ImageDirNode> m_imageDirNodes;

    QHash<QString, QString> m_filenameHash;

    friend uint qHash(const MLocalThemeDaemonClient::PixmapIdentifier &id);
    friend class tst_MLocalThemeDaemonClient; // Unit tests
};

#endif

