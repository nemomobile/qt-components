#include <QtDeclarative>

//QDI so we're in the scene and associated with a window
class QmlMxLikeScreen : public QDeclarativeItem
{

    Q_OBJECT
    public:
        QmlMxLikeScreen(QDeclarativeItem * parent=0);

    public slots:
        void dragPointMove(int x, int y, bool first=false);
        void dragPointResize(int x, int y, bool first=false);

    private:
        QPoint lastMovePoint;
        QPoint lastResizePoint;
};

QML_DECLARE_TYPE(QmlMxLikeScreen);
