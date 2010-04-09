#include "localaudiomodel.h"
#include "controller.h"
#include "qmlplayerview.h"
#include "qwidgetplayerview.h"

#include <QApplication>
#include <QTimer>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("declarativeDemo");

    if (argc == 1) {
        qWarning() << "Usage:" << argv[0] << " <path>";
        return 1;
    }

    const QLatin1String path = QLatin1String(argv[1]);
    LocalAudioModel model(path);
    QmlPlayerView view;
    PlayerController controller;

    controller.setModel(&model);
    controller.setView(&view);

    return app.exec();
}
