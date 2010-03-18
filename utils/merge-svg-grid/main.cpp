/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include <QtSvg>

static QImage svgToImage(const QString &fileName, QImage::Format format = QImage::Format_ARGB32)
{
    QSvgRenderer svg(fileName);
    QImage image(svg.defaultSize(), format);
    QPainter painter(&image);
    svg.render(&painter);
    painter.end();
    return image;
}

static QImage mergeSvgGrid(const QString &name)
{
    QString pf[9] = {
        "tl.svg",
        "l.svg",
        "bl.svg",
        "t.svg",
        "c.svg",
        "b.svg",
        "tr.svg",
        "r.svg",
        "br.svg"
    };

    QImage tl(svgToImage(name + pf[0]));
    QImage cl(svgToImage(name + pf[1]));
    QImage bl(svgToImage(name + pf[2]));
    QImage tc(svgToImage(name + pf[3]));
    QImage cc(svgToImage(name + pf[4]));
    QImage bc(svgToImage(name + pf[5]));
    QImage tr(svgToImage(name + pf[6]));
    QImage cr(svgToImage(name + pf[7]));
    QImage br(svgToImage(name + pf[8]));

    QImage result(tl.width() + tc.width() + tr.width(),
                  tl.height() + cl.height() + bl.height(),
                  tl.format());

    QPainter painter;
    painter.begin(&result);
    // left
    painter.drawImage(0, 0, tl);
    painter.drawImage(0, tl.height(), cl);
    painter.drawImage(0, tl.height() + cl.height(), bl);
    // center
    painter.drawImage(tl.width(), 0, tc);
    painter.drawImage(cl.width(), tc.height(), cc);
    painter.drawImage(bl.width(), tc.height() + cc.height(), bc);
    // right
    painter.drawImage(tl.width() + tc.width(), 0, tr);
    painter.drawImage(cl.width() + cc.width(), tr.height(), cr);
    painter.drawImage(bl.width() + bc.width(), tr.height() + cr.height(), br);
    painter.end();

    return result;
}
#if 0
class Preview : public QWidget
{

public:
    Preview(const QImage &image, QWidget *parent = 0)
        : QWidget(parent), m_image(image) {}
protected:
    void paintEvent(QPaintEvent *) {
        QPainter painter(this);
        painter.drawImage(0, 0, m_image);
    }

private:
    QImage m_image;
};
 #endif
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QList<QString> args = QApplication::arguments();
    QString name = args.count() > 1 ? args.at(1) : QFileDialog::getOpenFileName();
    name = name.left(name.lastIndexOf('_') + 1);
    QImage result = mergeSvgGrid(name);
#if 0
    Preview preview(result);
    preview.show();
#else
    QString path = args.count() > 2 ? args.at(2) : QDir::currentPath() + name + "m";
    result.save(path);
    return 0;
#endif
    return app.exec();
}
