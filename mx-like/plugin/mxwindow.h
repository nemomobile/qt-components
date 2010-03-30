#include <QtDeclarative>

//QDI so we're in the scene and associated with a window
class QmlMxLikeScreen : public QDeclarativeItem
{

    Q_OBJECT
    public:
        QmlMxLikeScreen(QDeclarativeItem * parent=0);

    public slots:
        void resetMove();
        void resetResize();
        void dragPointMove(int x, int y);
        void dragPointResize(int x, int y);

    private:
        bool moveValid, resizeValid;
        QPoint lastMovePoint;
        QPoint lastResizePoint;
};

QML_DECLARE_TYPE(QmlMxLikeScreen);
