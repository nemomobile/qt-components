#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <qdeclarativewindow.h>
#include <QtTest>

namespace tst_quickcomponentstest
{
    QString errorString(QDeclarativeComponent*);
    QObject* createComponentFromFile(QString const&, QString*,QDeclarativeEngine **engine=0);
};

inline
QString tst_quickcomponentstest::errorString(QDeclarativeComponent* component)
{
    QString out;
    if (!component) {
        out = "(null component)";
    }
    else if (component->isError()) {
        QString sep;
        foreach (QDeclarativeError const& e, component->errors()) {
            out += sep + e.toString();
            sep = "\n";
        }
    }

    return out;
}

inline
QObject* tst_quickcomponentstest::createComponentFromFile(QString const& filename, QString* errors, QDeclarativeEngine **engine)
{
    QDeclarativeWindow *window = new QDeclarativeWindow;
    if (engine)
        *engine = window->engine();
    window->engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");
    QDeclarativeComponent *component = new QDeclarativeComponent(window->engine());

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        if (errors) {
            *errors = QString("Could not open %1: %2").arg(filename).arg(file.errorString());
        }
        return 0;
    }

    component->setData( file.readAll(), QUrl() );

    QObject* out = component->create();
    if (!out) {
        if (errors) {
            *errors = QString("Could not create component from %1: %2").arg(filename).arg(errorString(component));
        }
    }

    return out;
}

