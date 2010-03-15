#include <QtGui/QtGui>
#include <QtDeclarative/QtDeclarative>
#include "qrangemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDeclarativeView view;
    QML_REGISTER_TYPE(Components, 1,0, RangeModel, QRangeModel);
    view.setSource(QUrl("qrc:slider.qml"));
    view.show();
    return app.exec();
}
