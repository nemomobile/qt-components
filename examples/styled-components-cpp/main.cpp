#include <QtDeclarative/QDeclarativeEngine>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsAnchorLayout>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsWidget>

#include "qgraphicsbutton.h"
#include "qmlstyle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ### Engine creation inside the style if no engine given...
    QDeclarativeEngine engine;
    QString style(qgetenv("COMPSTYLE"));
    QString path = QString("data/qmlstyle/%1").arg(style);
    new QmlStyle(&engine, path);

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 800, 480);

    QGraphicsWidget *w = new QGraphicsWidget(0, Qt::Window);
    w->setPos(20, 20);
    w->resize(300, 300);
    QGraphicsAnchorLayout *l = new QGraphicsAnchorLayout;
    w->setLayout(l);

    QGraphicsButton *a = new QGraphicsButton;
    QGraphicsButton *b = new QGraphicsButton;
    QGraphicsButton *c = new QGraphicsButton;

    // ### hack doesn't work...
    // a->setCheckable(true);
    // a->setAutoExclusive(true);
    // b->setCheckable(true);
    // b->setAutoExclusive(true);
    // c->setCheckable(true);
    // c->setAutoExclusive(true);

    // Horizontal
    l->addAnchors(a, l, Qt::Horizontal);
    l->addAnchor(l, Qt::AnchorLeft, b, Qt::AnchorLeft);
    l->addAnchor(b, Qt::AnchorRight, c, Qt::AnchorLeft);
    l->addAnchor(c, Qt::AnchorRight, l, Qt::AnchorRight);

    // Vertical
    l->addAnchor(a, Qt::AnchorTop, l, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorTop, a, Qt::AnchorBottom);
    l->addAnchor(c, Qt::AnchorTop, b, Qt::AnchorTop);
    l->addAnchor(b, Qt::AnchorBottom, l, Qt::AnchorBottom);
    l->addAnchor(c, Qt::AnchorBottom, l, Qt::AnchorBottom);

    b->setPreferredWidth(200);
    b->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    c->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    scene.addItem(w);
    scene.setBackgroundBrush(Qt::darkGreen);
    QGraphicsView view(&scene);
    view.show();

    return app.exec();
}

