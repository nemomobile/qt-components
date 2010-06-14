/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Components project
** on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QDeclarativeExtensionPlugin>

#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QDeclarativeImageProvider>
#include <QImageReader>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>

class PhotoProvider : public QDeclarativeImageProvider
{
public:
    QImage request(const QString &id, QSize *size, const QSize &req_size) {

        //QPixmap pixmap(id);
        //return pixmap.toImage();
        qDebug() << "request: " << id;
        QImageReader imgReader(id);
        *size = imgReader.size();
        size->scale (req_size, Qt::KeepAspectRatio);
        imgReader.setScaledSize (*size);
        return imgReader.read();

    }
};


class PhotoModelPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    Q_PROPERTY(QStringList list READ list);
    Q_PROPERTY(QString path READ path WRITE setPath);
    Q_PROPERTY(int size READ size);

public:
    void registerTypes(const char *uri) {
        Q_UNUSED(uri);
    }

    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        Q_UNUSED(uri);

        engine->addImageProvider("photos", new PhotoProvider);
        QDeclarativeContext *context = engine->rootContext();
        context->setContextProperty("photoModel", this);
    }

    QString path() const {
        return m_path;
    }

    void setPath(const QString &path) {
        if (m_path == path)
            return;

        m_path = path;
        reloadPhotos(path);
    }

    QStringList list() const {
        return m_photosFiles;
    }

    int size() const {
        return m_photosFiles.size();
    }

protected:
    void reloadPhotos(const QString &path) {
        m_photosFiles.clear();
        QDir dir(path);

        dir.setNameFilters(QStringList() << "*.jpg" << "*.png");
        dir.setFilter(QDir::Files);

        QList<QFileInfo> list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            m_photosFiles << list[i].absoluteFilePath();
        }
    }

private:
    QStringList m_photosFiles;
    QString m_path;
};

#include "photomodel.moc"

Q_EXPORT_PLUGIN(PhotoModelPlugin);
