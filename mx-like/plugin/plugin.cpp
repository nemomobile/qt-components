#include <QtDeclarative/qdeclarativeextensionplugin.h>
#include <QtDeclarative/qdeclarative.h>

#include "mxwindow.h"

class QmlMxLikeModule : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        Q_UNUSED(uri);//But we register to it for implicit discovery?
        qmlRegisterType<QmlMxLikeScreen>(uri,1,0,"WindowModel");
    }
};

#include "plugin.moc"
Q_EXPORT_PLUGIN2(qmlmxlikemodule, QmlMxLikeModule);
