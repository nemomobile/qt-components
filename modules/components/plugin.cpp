#include <QtDeclarative>

#include "qanglemodel.h"
#include "qbuttonmodel.h"
#include "qdatetimemodel.h"
#include "qlineeditmodel.h"
#include "qprogressbarmodel.h"
#include "qrangemodel.h"

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
        qmlRegisterType<QAngleModel>(uri, 1, 0, "AngleModel");
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QDateTimeModel>(uri, 1, 0, "DateTimeModel");
        qmlRegisterType<QLineEditModel>(uri, 1, 0, "LineEditModel");
        qmlRegisterType<QLineEditLayoutHelper>(uri, 1, 0, "LineEditLayoutHelper");
        qmlRegisterType<QLineEditEventHelper>(uri, 1, 0, "LineEditEventHelper");
        qmlRegisterType<QProgressBarModel>(uri, 1, 0, "ProgressBarModel");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");

        qmlRegisterType<QDeclarativeButton>(uri, 1, 0, "StyleButton");
        qmlRegisterType<QDeclarativeSlider>(uri, 1, 0, "StyleSlider");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(componentsqmlplugin, ComponentsQmlPlugin);
