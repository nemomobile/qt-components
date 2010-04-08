#include <QtDeclarative>

#include "qbuttonmodel.h"
#include "qlineeditmodel.h"
#include "qrangemodel.h"
#include "qdatetimemodel.h"
#include "qprogressbarmodel.h"
#include "qdeclarativebutton.h"
#include "qdeclarativeslider.h"

#include "qmlstyle.h"
#include "teststyle.h"


class ComponentsQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        // ###
        QString style(qgetenv("COMPSTYLE"));
        if (style == "") {
            new TestStyle(engine);
        } else {
            QString path = QString("data/qmlstyle/%1").arg(style);
            qWarning() << "Loading theme from: " << path;
            new QmlStyle(path, engine, engine);
        }
    }

    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("Components"));
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QLineEditModel>(uri, 1, 0, "LineEditModel");
        qmlRegisterType<QLineEditEventHelper>(uri, 1, 0, "LineEditEventHelper");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");
        qmlRegisterType<QDateTimeModel>(uri, 1, 0, "DateTimeModel");
        qmlRegisterType<QProgressBarModel>(uri, 1, 0, "ProgressBarModel");
        qmlRegisterType<QDeclarativeButton>(uri, 1, 0, "StyleButton");
        qmlRegisterType<QDeclarativeSlider>(uri, 1, 0, "StyleSlider");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(componentsqmlplugin, ComponentsQmlPlugin);
