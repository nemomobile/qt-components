

#include <QDeclarativeExtensionPlugin>

#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QDeclarativeImageProvider>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

class PhotoProvider : public QDeclarativeImageProvider
{
public:
    QImage request(const QString &id) {
        QPixmap pixmap(id);
        return pixmap.toImage();
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

    QString path() {
        return m_path;
    }

    void setPath(const QString &path) {
        if (m_path == path)
            return;

        m_path = path;
        reloadPhotos(path);
    }

    QStringList list() {
        return m_photosFiles;
    }

    int size() {
        return m_photosFiles.size();
    }

protected:
    void reloadPhotos(const QString &path) {
        m_photosFiles.clear();
        QDir dir(path);

        dir.setNameFilters(QStringList() << "*.jpg");
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
