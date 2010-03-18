#include <QtCore>
#include <QtGui>
#include <QSvgRenderer>

int main(int argc, char **argv)
{

    const char *filenamebase = argv[1];
    const char *parts[] = {"tl", "t", "tr", "l", "c", "r", "bl", "b", "br"};

    QImage image(30, 30, QImage::Format_ARGB32);
    QPainter painter(&image);

    QSvgRenderer renderer;
    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            int index = 3 * row + col;
            QString path;
            path.sprintf("%s_%s.svg", filenamebase, parts[index]);

            renderer.load(path);
            renderer.render(&painter, QRect(col * 10, row * 10, 10, 10));
        }
    }

    QString path;
    path.sprintf("%s.png", filenamebase);
    image.save(path);

    return 0;
}
