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

#include "mlocalthemedaemonclient.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>

MLocalThemeDaemonClient::MLocalThemeDaemonClient(const QString &path, QObject *parent) :
    MAbstractThemeDaemonClient(parent),
    m_path(path),
    m_pixmapCache(),
    m_imageDirNodes()
{
    if (m_path.isEmpty()) {
        m_path = qgetenv("M_THEME_DIR");
        if (m_path.isEmpty()) {
#ifdef THEME_DIR
            m_path = THEME_DIR;
#else
            qWarning() << "No theme path is provided for MLocalThemeDaemonClient";
#endif
        }

        m_path += QDir::separator()
                 + QLatin1String("base") + QDir::separator()
                 + QLatin1String("meegotouch");
    }

    if (m_path.endsWith(QDir::separator())) {
        m_path.truncate(m_path.length() - 1);
    }

    m_imageDirNodes.append(ImageDirNode("icons" , QStringList() << ".svg" << ".png" << ".jpg"));
    m_imageDirNodes.append(ImageDirNode("images", QStringList() << ".png" << ".jpg"));
    m_imageDirNodes.append(ImageDirNode("svg",    QStringList() << ".svg"));
}

MLocalThemeDaemonClient::~MLocalThemeDaemonClient()
{
}

QPixmap MLocalThemeDaemonClient::requestPixmap(const QString &id, const QSize &requestedSize)
{
    QPixmap pixmap;

    QSize size = requestedSize;
    if (size.width() < 1) {
        size.rwidth() = 0;
    }
    if (size.height() < 1) {
        size.rheight() = 0;
    }

    const PixmapIdentifier pixmapId(id, size);
    pixmap = m_pixmapCache.value(pixmapId);
    if (pixmap.isNull()) {
        // The pixmap is not cached yet. Decode the image and
        // store it into the cache as pixmap.
        const QImage image = readImage(id);
        if (!image.isNull()) {
            pixmap = QPixmap::fromImage(image);
            if (requestedSize.isValid() && (pixmap.size() != requestedSize)) {
                pixmap = pixmap.scaled(requestedSize);
            }

            m_pixmapCache.insert(pixmapId, pixmap);
        }
    }
    return pixmap;
}

QImage MLocalThemeDaemonClient::readImage(const QString &id) const
{
    foreach (const ImageDirNode &imageDirNode, m_imageDirNodes) {
        const QString filePath = m_path + QDir::separator()
                                 + imageDirNode.directory + QDir::separator()
                                 + id;

        foreach (const QString &suffix, imageDirNode.suffixList) {
            QImage image(filePath + suffix);
            if (!image.isNull()) {
                return image;
            }
        }
    }
    return QImage();
}

MLocalThemeDaemonClient::PixmapIdentifier::PixmapIdentifier() :
    imageId(), size()
{
}

MLocalThemeDaemonClient::PixmapIdentifier::PixmapIdentifier(const QString &imageId, const QSize &size) :
    imageId(imageId), size(size)
{
}

bool MLocalThemeDaemonClient::PixmapIdentifier::operator==(const PixmapIdentifier &other) const
{
    return imageId == other.imageId && size == other.size;
}

bool MLocalThemeDaemonClient::PixmapIdentifier::operator!=(const PixmapIdentifier &other) const
{
    return imageId != other.imageId || size != other.size;
}

MLocalThemeDaemonClient::ImageDirNode::ImageDirNode(const QString &directory, const QStringList &suffixList) :
    directory(directory),
    suffixList(suffixList)
{
}

uint qHash(const MLocalThemeDaemonClient::PixmapIdentifier &id)
{
    using ::qHash;

    const uint idHash     = qHash(id.imageId);
    const uint widthHash  = qHash(id.size.width());
    const uint heightHash = qHash(id.size.height());

    // Twiddle the bits a little, taking a cue from Qt's own qHash() overloads
    return idHash ^ (widthHash << 8) ^ (widthHash >> 24) ^ (heightHash << 24) ^ (heightHash >> 8);
}
