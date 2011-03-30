#include <QApplication>
#include <QDeclarativeView>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;

#ifndef Q_OS_SYMBIAN
    QDir::setCurrent(app.applicationDirPath());
#endif

    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();

    return app.exec();
}
