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
            //qWarning() << "No theme path is provided for MLocalThemeDaemonClient";

            #ifdef Q_OS_WIN
            m_path = "c:\\";
            #else
            m_path = "/usr/share/themes";
            #endif
        }

        m_path += QDir::separator()
                + QLatin1String("blanco") + QDir::separator()
                + QLatin1String("meegotouch");
    }

    if (m_path.endsWith(QDir::separator())) {
        m_path.truncate(m_path.length() - 1);
    }

    m_imageDirNodes.append(ImageDirNode("icons" , QStringList() << ".svg" << ".png" << ".jpg"));
    m_imageDirNodes.append(ImageDirNode(QLatin1String("images") + QDir::separator() + QLatin1String("theme"), QStringList() << ".png" << ".jpg"));
    m_imageDirNodes.append(ImageDirNode(QLatin1String("images") + QDir::separator() + QLatin1String("backgrounds"), QStringList() << ".png" << ".jpg"));

    buildHash(m_path + QDir::separator() + "icons", QStringList() << "*.svg" << "*.png" << "*.jpg");
    buildHash(m_path + QDir::separator() + "images" + QDir::separator() + "theme", QStringList() << "*.png" << "*.jpg");
    buildHash(m_path + QDir::separator() + "images" + QDir::separator() + "backgrounds", QStringList() << "*.png" << "*.jpg");

    qDebug() << "LocalThemeDaemonClient: Looking for assets in" << m_path;
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
        foreach (const QString &suffix, imageDirNode.suffixList) {

            QString imageFilePathString = m_filenameHash.value(id + suffix);
            if (!imageFilePathString.isNull()) {
                imageFilePathString.append(QDir::separator() + id + suffix);
            }

            QImage image(imageFilePathString);
            if (!image.isNull()) {
                return image;
            }
        }
    }

    return QImage();
}

QString MLocalThemeDaemonClient::findFileRecursively(const QDir& rootDir, const QString& name)
{
    QStringList files = rootDir.entryList(QStringList(name));
    if(files.length() > 0)
        return rootDir.filePath(files[0]);

    QStringList dirList = rootDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach(const QString nextDirString, dirList){
        QDir nextDir(rootDir.absolutePath() + QDir::separator() + nextDirString);
        QString nextFile = findFileRecursively(nextDir, name);
        if(!nextFile.isNull())
            return nextFile;
    }

    return QString();
}

void MLocalThemeDaemonClient::buildHash(const QDir& rootDir, const QStringList& nameFilter)
{
    QDir rDir = rootDir;
    rDir.setNameFilters(nameFilter);
    QStringList files = rDir.entryList(QDir::Files);
    foreach (const QString filename, files) {
        m_filenameHash.insert(filename, rootDir.absolutePath());
    }

    QStringList dirList = rootDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach(const QString nextDirString, dirList){
        QDir nextDir(rootDir.absolutePath() + QDir::separator() + nextDirString);
        buildHash(nextDir, nameFilter);
    }
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
