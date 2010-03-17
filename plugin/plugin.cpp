#include <QtDeclarative>

#include "qbuttonmodel.h"
#include "qrangemodel.h"

class ComponentsQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("Components"));
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(componentsqmlplugin, ComponentsQmlPlugin);
