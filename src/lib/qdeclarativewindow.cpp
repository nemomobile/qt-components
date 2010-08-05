/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include "qdeclarativewindow.h"

#include <QApplication>
#include <qgl.h>
#include <QtDeclarative>

#ifdef Q_COMPONENTS_MEEGO
#include <MComponentData>
#endif

class QDeclarativeWindowPrivate
{
public:
    QDeclarativeWindowPrivate(QDeclarativeWindow *qq);
    ~QDeclarativeWindowPrivate();
    void execute();

    QDeclarativeWindow *q;

    QGraphicsView *view;
    QGraphicsScene scene;
    QUrl source;

    QDeclarativeEngine engine;
    QDeclarativeComponent *component;
    QPointer<QDeclarativeItem> root;

};

QDeclarativeWindowPrivate::QDeclarativeWindowPrivate(QDeclarativeWindow *qq)
    : q(qq), view(0), component(0)
{
#ifdef Q_COMPONENTS_MEEGO
    qApp->setProperty("NoMStyle", true);
    if(!MComponentData::instance()) {
        // This is a workaround because we can't use a default
        // constructor for MComponentData
        int argc = 1;
        char *argv0 = "meegotouch";
        (void) new MComponentData(argc, &argv0);
    }
#endif

    view = new QGraphicsView(&scene, 0);

#ifdef Q_COMPONENTS_MEEGO
    view->setWindowFlags(Qt::Window
                   | Qt::CustomizeWindowHint
                   | Qt::FramelessWindowHint);
    view->resize(QApplication::desktop()->screenGeometry().width(),
                 QApplication::desktop()->screenGeometry().height());
#endif

    view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameStyle(0);

    // These seem to give the best performance
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    view->viewport()->setFocusPolicy(Qt::NoFocus);
    view->setFocusPolicy(Qt::StrongFocus);

    view->setViewport(new QGLWidget);

    scene.setStickyFocus(true);  //### needed for correct focus handling

    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
}


QDeclarativeWindowPrivate::~QDeclarativeWindowPrivate()
{
    delete component;
    delete view;
}

void QDeclarativeWindowPrivate::execute()
{
    if (root) {
        delete root;
        root = 0;
    }
    if (component) {
        delete component;
        component = 0;
    }
    if (!source.isEmpty()) {
        component = new QDeclarativeComponent(&engine, source, q);
        if (!component->isLoading()) {
            q->continueExecute();
        } else {
            QObject::connect(component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), q, SLOT(continueExecute()));
        }
    }
}

/*!
  \internal
 */
void QDeclarativeWindow::continueExecute()
{
    disconnect(d->component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(continueExecute()));

    if (d->component->isError()) {
        QList<QDeclarativeError> errorList = d->component->errors();
        foreach (const QDeclarativeError &error, errorList) {
            qWarning() << error;
        }
        emit statusChanged(status());
        return;
    }

    QObject *obj = d->component->create();

    if(d->component->isError()) {
        QList<QDeclarativeError> errorList = d->component->errors();
        foreach (const QDeclarativeError &error, errorList) {
            qWarning() << error;
        }
        emit statusChanged(status());
        return;
    }

    setRootObject(obj);

    emit statusChanged(status());
}






QDeclarativeWindow::QDeclarativeWindow()
{
    d = new QDeclarativeWindowPrivate(this);
}

QDeclarativeWindow::QDeclarativeWindow(const QUrl &source)
{
    d = new QDeclarativeWindowPrivate(this);
    setSource(source);
}

QDeclarativeWindow::~QDeclarativeWindow()
{
    delete d;
    d = 0;
}


/*! \property QDeclarativeWindow::source
  \brief The URL of the source of the QML component.

  Changing this property causes the QML component to be reloaded.

    Ensure that the URL provided is full and correct, in particular, use
    \l QUrl::fromLocalFile() when loading a file from the local filesystem.
 */

/*!
    Sets the source to the \a url, loads the QML component and instantiates it.

    Ensure that the URL provided is full and correct, in particular, use
    \l QUrl::fromLocalFile() when loading a file from the local filesystem.

    Calling this methods multiple times with the same url will result
    in the QML being reloaded.
 */
void QDeclarativeWindow::setSource(const QUrl& url)
{
    d->source = url;
    d->execute();
}

/*!
  Returns the source URL, if set.

  \sa setSource()
 */
QUrl QDeclarativeWindow::source() const
{
    return d->source;
}

/*!
  Returns a pointer to the QDeclarativeEngine used for instantiating
  QML Components.
 */
QDeclarativeEngine* QDeclarativeWindow::engine()
{
    return &d->engine;
}

/*!
  This function returns the root of the context hierarchy.  Each QML
  component is instantiated in a QDeclarativeContext.  QDeclarativeContext's are
  essential for passing data to QML components.  In QML, contexts are
  arranged hierarchically and this hierarchy is managed by the
  QDeclarativeEngine.
 */
QDeclarativeContext* QDeclarativeWindow::rootContext()
{
    return d->engine.rootContext();
}


/*!
  Returns the view's root \l {QGraphicsObject} {item}.
 */
QGraphicsObject *QDeclarativeWindow::rootObject() const
{
    return d->root;
}

/*! \fn void QDeclarativeWindow::statusChanged(QDeclarativeView::Status status)
    This signal is emitted when the component's current \a status changes.
*/

/*!
    \property QDeclarativeWindow::status
    The component's current \l{QDeclarativeWindow::Status} {status}.
*/
QDeclarativeWindow::Status QDeclarativeWindow::status() const
{
    if (!d->component)
        return QDeclarativeWindow::Null;

    return QDeclarativeWindow::Status(d->component->status());
}


/*!
  \internal
*/
void QDeclarativeWindow::setRootObject(QObject *obj)
{
    if (d->root == obj)
        return;
    d->root = qobject_cast<QDeclarativeItem *>(obj);
    if (!d->root) {
        qWarning() << "QDeclarativeView only supports loading of root objects that derive from QDeclarativeItem";
        return;
    }

    d->scene.addItem(d->root);
}

QWidget *QDeclarativeWindow::window()
{
    return d->view;
}

