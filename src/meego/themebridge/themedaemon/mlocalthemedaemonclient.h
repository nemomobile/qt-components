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

#ifndef MLOCALTHEMEDAEMONCLIENT_H
#define MLOCALTHEMEDAEMONCLIENT_H

#include <themedaemon/mabstractthemedaemonclient.h>

#include <QHash>
#include <QPixmap>
#include <QString>

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

    friend uint qHash(const MLocalThemeDaemonClient::PixmapIdentifier &id);
    friend class tst_MLocalThemeDaemonClient; // Unit tests
};

#endif

