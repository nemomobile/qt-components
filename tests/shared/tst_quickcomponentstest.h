#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QtTest>

namespace tst_quickcomponentstest
{
    QString errorString(QDeclarativeComponent*);
    QObject* createComponentFromFile(QString const&, QString*,QDeclarativeView **view=0);
    QObject* createComponentFromString(QString const&, QString*, QDeclarativeEngine **engine=0);
    QDeclarativeView *createDeclarativeView(const QString& source);
}

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
QObject* tst_quickcomponentstest::createComponentFromFile(QString const& filename, QString* errors, QDeclarativeView **view)
{
    // view usage priority
    // #1 *view points to a view instance - use that and ownership is not transferred
    // #2 *view points to null - create new view and pass the ownership to caller
    // #3 view points to null - create new view and let the application quit destroy it
    QDeclarativeView *usedView = 0;
    if (view) {
        if (!*view)
            *view  = new QDeclarativeView;
        usedView = *view;
    } else {
        usedView = new QDeclarativeView;
    }
    //usedView->setProperty("orientationMethod", 1);

    usedView->engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");
    QDeclarativeComponent component(usedView->engine());

    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        if (errors) {
            *errors = QString("Could not open %1: %2").arg(filename).arg(file.errorString());
        }
        return 0;
    }

    component.setData( file.readAll(), QUrl() );

    QObject* out = component.create();
    if (!out) {
        if (errors) {
            *errors = QString("Could not create component from %1: %2").arg(filename).arg(errorString(&component));
        }
    }
    if (!view)
        QObject::connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), usedView, SLOT(deleteLater()));

    return out;
}

inline
QObject* tst_quickcomponentstest::createComponentFromString(QString const &qml, QString *errors, QDeclarativeEngine **engine)
{
    QDeclarativeView *window = new QDeclarativeView;
    //window->setProperty("orientationMethod", 1);
    if (engine)
        *engine = window->engine();
    window->engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");
    QDeclarativeComponent *component = new QDeclarativeComponent(window->engine());
    component->setData(qml.toUtf8(), QUrl());
    QObject* out = component->create();
    if (!out) {
        if (errors) {
            *errors = QString("Could not create component from %1: %2").arg(qml).arg(errorString(component));
        }
    }
    return out;
}

inline
QDeclarativeView* tst_quickcomponentstest::createDeclarativeView(const QString& source)
{
    QDeclarativeView* view = new QDeclarativeView(0);
    //view->setProperty("orientationMethod", 1);
    view->engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");
    view->setSource(QUrl::fromLocalFile(source));
    view->show();

    view->requestActivateWindow();
    return view;
}
