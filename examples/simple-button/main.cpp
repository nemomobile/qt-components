#include <QtGui/QtGui>
#include <QtDeclarative/QtDeclarative>
#include "qbuttonmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;
    QML_REGISTER_TYPE(Components, 1,0, ButtonModel, QButtonModel);
    view.setSource(QUrl("main.qml"));
    view.show();
    return app.exec();
}
