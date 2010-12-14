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

#ifndef SDECLARATIVEWINDOWDECORATION_H
#define SDECLARATIVEWINDOWDECORATION_H

#include <QDeclarativeItem>

class SDeclarativeWindowDecorationPrivate;

class SDeclarativeWindowDecoration : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(int orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(bool backButtonVisible READ isBackButtonVisible WRITE setBackButtonVisible NOTIFY backButtonVisibleChanged)
    Q_PROPERTY(bool statusBarVisible READ isStatusBarVisible WRITE setStatusBarVisible NOTIFY statusBarVisibleChanged)
    Q_PROPERTY(bool titleBarVisible READ isTitleBarVisible WRITE setTitleBarVisible NOTIFY titleBarVisibleChanged)
    Q_PROPERTY(qreal topDecorationHeight READ topDecorationHeight WRITE setTopDecorationHeight NOTIFY topDecorationHeightChanged)
    Q_PROPERTY(qreal bottomDecorationHeight READ bottomDecorationHeight WRITE setBottomDecorationHeight NOTIFY bottomDecorationHeightChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QDeclarativeItem *toolBar READ toolBar WRITE setToolBar NOTIFY toolBarChanged)

public:
    explicit SDeclarativeWindowDecoration(QDeclarativeItem *parent = 0);
    ~SDeclarativeWindowDecoration();

    int orientation() const;
    void setOrientation(int orientation);

    bool isBackButtonVisible() const;
    void setBackButtonVisible(bool backButtonVisible);

    bool isStatusBarVisible() const;
    void setStatusBarVisible(bool statusBarVisible);

    bool isTitleBarVisible() const;
    void setTitleBarVisible(bool titleBarVisible);

    qreal topDecorationHeight() const;
    void setTopDecorationHeight(qreal topDecorationHeight);

    qreal bottomDecorationHeight() const;
    void setBottomDecorationHeight(qreal bottomDecorationHeight);

    QString title() const;
    void setTitle(const QString &title);

    QDeclarativeItem *toolBar() const;
    void setToolBar(QDeclarativeItem *toolBar);

    Q_INVOKABLE bool isLandscape() const;

#ifndef Q_OS_SYMBIAN
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
#endif

signals:
    void minimize();
    void quit();
    void backClicked();

    void orientationChanged(int orientation);
    void backButtonVisibleChanged(bool show);
    void statusBarVisibleChanged(bool visible);
    void titleBarVisibleChanged(bool visible);
    void topDecorationHeightChanged(qreal height);
    void bottomDecorationHeightChanged(qreal height);
    void titleChanged(const QString &title);
    void toolBarChanged(QDeclarativeItem *toolBar);

protected:
    QScopedPointer<SDeclarativeWindowDecorationPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_doUpdateFullScreen())
    Q_PRIVATE_SLOT(d_func(), void _q_doUpdateCba())
    Q_PRIVATE_SLOT(d_func(), void _q_desktopWorkareaChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_optionsSelected())
    Q_PRIVATE_SLOT(d_func(), void _q_exitSelected())
    Q_PRIVATE_SLOT(d_func(), void _q_backSelected())

    Q_DISABLE_COPY(SDeclarativeWindowDecoration)
    Q_DECLARE_PRIVATE(SDeclarativeWindowDecoration)
};

QML_DECLARE_TYPE(SDeclarativeWindowDecoration)

#endif // SDECLARATIVEWINDOWDECORATION_H
