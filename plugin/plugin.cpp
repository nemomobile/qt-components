#include <QtDeclarative>

#include "qbuttonmodel.h"
#include "qlineeditmodel.h"
#include "qrangemodel.h"
#include "qdatetimemodel.h"
#include "qprogressbarmodel.h"
#include "qdeclarativebutton.h"

#include "qmlstyle.h"
#include "teststyle.h"

// Test declarative item button
#include "button.h"


class ComponentsQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void initializeEngine(QDeclarativeEngine *engine, const char *uri) {
        // ###
        QString style(qgetenv("COMPSTYLE"));
        if (style == "qml")
            new QmlStyle(engine, "data/qmlstyle");
        else
            new TestStyle;
    }

    void registerTypes(const char *uri) {
        Q_ASSERT(uri == QLatin1String("Components"));
        qmlRegisterType<QButtonModel>(uri, 1, 0, "ButtonModel");
        qmlRegisterType<QLineEditModel>(uri, 1, 0, "LineEditModel");
        qmlRegisterType<QLineEditEventHelper>(uri, 1, 0, "LineEditEventHelper");
        qmlRegisterType<QRangeModel>(uri, 1, 0, "RangeModel");
        qmlRegisterType<QDateTimeModel>(uri, 1, 0, "DateTimeModel");
        qmlRegisterType<QProgressBarModel>(uri, 1, 0, "ProgressBarModel");
        qmlRegisterType<Button>(uri, 1, 0, "Botao");
        qmlRegisterType<QDeclarativeButton>(uri, 1, 0, "StyleButton");
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(componentsqmlplugin, ComponentsQmlPlugin);
