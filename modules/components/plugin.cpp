#include <QtDeclarative>

#include "qbuttonmodel.h"
#include "qlineeditmodel.h"
#include "qrangemodel.h"
#include "qdatetimemodel.h"
#include "qprogressbarmodel.h"


class ComponentsQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("Components"));
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QLineEditModel>(uri, 1, 0, "LineEditModel");
        qmlRegisterType<QLineEditEventHelper>(uri, 1, 0, "LineEditEventHelper");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");
        qmlRegisterType<QDateTimeModel>(uri, 1, 0, "DateTimeModel");
        qmlRegisterType<QProgressBarModel>(uri, 1, 0, "ProgressBarModel");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(componentsqmlplugin, ComponentsQmlPlugin);
