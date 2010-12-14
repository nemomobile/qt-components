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

#include "sdeclarativewindowdecoration.h"
#include "sdeclarativescreen.h"

#include <QApplication>
#include <QGraphicsView>
#ifdef Q_OS_SYMBIAN
#include <QDesktopWidget>
#include <eikenv.h>
#include <eikaufty.h>
#include <eikspane.h>
#include <avkon.rsg>
#else
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFileInfo>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#endif

#define Q_DEBUG_DECORATION
#ifdef Q_DEBUG_DECORATION
#include <QDebug>
#endif

class SDeclarativeWindowDecorationPrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeWindowDecoration)

public:
    SDeclarativeWindowDecorationPrivate(SDeclarativeWindowDecoration *qq) :
        q_ptr(qq),
        backButtonVisible(false),
        topDecorationHeight(0),
        bottomDecorationHeight(0),
        toolBar(0),
        optionsAction(0),
        backAction(0),
        exitAction(0),
        rootWidget(0),
        updatingFullScreen(false),
        updatingCba(false)
    {
    }

    ~SDeclarativeWindowDecorationPrivate()
    {
    }

    QAction *findActionWithSoftkeyRole(QAction::SoftKeyRole role) const
    {
        QList<QAction*> actions = rootWidget->actions();
        foreach(QAction *a, actions) {
            if (a->softKeyRole() == role) {
                return a;
            }
        }
        return 0;
    }

    QAction *leftSoftkey() const
    {
        return findActionWithSoftkeyRole(QAction::PositiveSoftKey);
    }

    QAction *rightSoftkey() const
    {
        return findActionWithSoftkeyRole(QAction::NegativeSoftKey);
    }

    void updateCba();
    void _q_doUpdateCba();

    void updateFullScreen();
    void _q_doUpdateFullScreen();

    void _q_desktopWorkareaChanged();
    void _q_optionsSelected();
    void _q_exitSelected();
    void _q_backSelected();

    SDeclarativeWindowDecoration *q_ptr;
    int orientation;
    bool backButtonVisible;
    bool statusBarVisible;
    bool titleBarVisible;
    qreal topDecorationHeight;
    qreal bottomDecorationHeight;
    QString title;
    QDeclarativeItem *toolBar;
    QAction *optionsAction;
    QAction *backAction;
    QAction *exitAction;
    QWidget *rootWidget;
    bool updatingFullScreen;
    bool updatingCba;
};

void SDeclarativeWindowDecorationPrivate::updateCba()
{
    Q_Q(SDeclarativeWindowDecoration);
    // avoid flickering by updating asyncronously
    if (!updatingCba) {
        QMetaObject::invokeMethod(q, "_q_doUpdateCba", Qt::QueuedConnection);
        updatingCba = true;
    }
}

void SDeclarativeWindowDecorationPrivate::_q_doUpdateCba()
{
    updatingCba = false;

    exitAction->setSoftKeyRole(QAction::NoSoftKey);
    backAction->setSoftKeyRole(QAction::NoSoftKey);
    optionsAction->setSoftKeyRole(QAction::NoSoftKey);

    optionsAction->setSoftKeyRole(QAction::PositiveSoftKey);
    if (backButtonVisible) {
        backAction->setSoftKeyRole(QAction::NegativeSoftKey);
    } else {
        exitAction->setSoftKeyRole(QAction::NegativeSoftKey);
    }
}

void SDeclarativeWindowDecorationPrivate::updateFullScreen()
{
    Q_Q(SDeclarativeWindowDecoration);
    // avoid flickering by updating asyncronously
    if (!updatingFullScreen) {
        QMetaObject::invokeMethod(q, "_q_doUpdateFullScreen" , Qt::QueuedConnection);
        updatingFullScreen = true;
    }
}

void SDeclarativeWindowDecorationPrivate::_q_doUpdateFullScreen()
{
    Q_Q(SDeclarativeWindowDecoration);
    updatingFullScreen = false;

#ifdef Q_OS_SYMBIAN
    if (!statusBarVisible && !titleBarVisible) {
        rootWidget->showFullScreen();
    } else {
        rootWidget->showMaximized();
        updateCba();
    }
#else
    static QObject *screen = 0;
    if (!screen) {
        QDeclarativeContext *context = QDeclarativeEngine::contextForObject(q);
        screen = qVariantValue<QObject *>(context->contextProperty("screen"));

        QObject::connect(screen,SIGNAL(displayChanged()), q, SLOT(_q_doUpdateFullScreen()));
    }

    if (!statusBarVisible && !titleBarVisible) {
        q->setTopDecorationHeight(0);
        q->setBottomDecorationHeight(0);
    } else {
        const qreal unit = screen->property("unit").value<qreal>();
        // Emulate Avkon decorator sizes in desktop.
        // TODO: This will be removed when decorators are done with QML
        //
        // Magic numbers come from reference device and are calculated using 6.75 as a unit value:
        //
        //                  Portrait    Landscape
        // Top decoration:      45          92
        // Magic number:        6.7         13.6
        //
        // Bottom decoration:   45          61
        // Magic number:        6.7         9.0
        q->setTopDecorationHeight(q->isLandscape() ? qRound(6.7 * unit) : qRound(13.6 * unit));
        q->setBottomDecorationHeight(q->isLandscape() ? qRound(6.7 * unit) : qRound(9.0 * unit));
    }
    q->update();
#endif // Q_OS_SYMBIAN
}

void SDeclarativeWindowDecorationPrivate::_q_desktopWorkareaChanged()
{
#ifdef Q_OS_SYMBIAN
    Q_Q(SDeclarativeWindowDecoration);
    // use flat status pane layout. this removes the space needed for the
    // tabs/navigation page in portrait, which we do not currently use.
    static CEikonEnv *eikonEnv = 0;
    if (!eikonEnv) {
        eikonEnv = CEikonEnv::Static();
        // first event syncronously so that the wrong status pane layout doesn't get drawn
        // next events asyncronously so that the screenGeometry is updated
        QObject::disconnect(QApplication::desktop(), SIGNAL(workAreaResized(int)), q, SLOT(_q_desktopWorkareaChanged()));
        QObject::connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), q, SLOT(_q_desktopWorkareaChanged()), Qt::QueuedConnection);
    }

    MEikAppUiFactory *eikAppUiFactory = eikonEnv->AppUiFactory();
    if (eikAppUiFactory) {
        // the decorators are allocated in QWidget::show, so this
        // if the first possible point to access it i.e. this
        // cannot be done in the constructor.
        CEikStatusPane *eikStatusPane = eikAppUiFactory->StatusPane();
        if (eikStatusPane && eikStatusPane->CurrentLayoutResId()
                == R_AVKON_STATUS_PANE_LAYOUT_USUAL_EXT) {
            QT_TRAP_THROWING(eikStatusPane->SwitchLayoutL(R_AVKON_STATUS_PANE_LAYOUT_USUAL_FLAT));
            return;
        }
    }

    const QRect screenGeometry = QApplication::desktop()->screenGeometry();
    const TRect newApplicationAreaSymbian = static_cast<CEikAppUi*>(eikonEnv->AppUi())->ClientRect();
    QRect newApplicationArea(newApplicationAreaSymbian.iTl.iX, newApplicationAreaSymbian.iTl.iY, newApplicationAreaSymbian.Width(), newApplicationAreaSymbian.Height());

#ifdef Q_DEBUG_DECORATION
    qDebug() << "SDeclarativeWindowDecoration::desktopWorkareaChanged(): " << newApplicationArea;
#endif
    // keep top decorator as 0 height and set the whole offset to bottom decoration
    q->setTopDecorationHeight(0);
    q->setBottomDecorationHeight(screenGeometry.height() - newApplicationArea.height());
#endif
}

void SDeclarativeWindowDecorationPrivate::_q_optionsSelected()
{
#ifdef Q_DEBUG_DECORATION
    qDebug() << "OPTIONS";
#endif
}

void SDeclarativeWindowDecorationPrivate::_q_exitSelected()
{
    Q_Q(SDeclarativeWindowDecoration);
    emit q->quit();
}

void SDeclarativeWindowDecorationPrivate::_q_backSelected()
{
    Q_Q(SDeclarativeWindowDecoration);
    emit q->backClicked();
}

SDeclarativeWindowDecoration::SDeclarativeWindowDecoration(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), d_ptr(new SDeclarativeWindowDecorationPrivate(this))
{
    Q_D(SDeclarativeWindowDecoration);
#ifdef Q_OS_SYMBIAN
    QObject::connect(QApplication::desktop(), SIGNAL(workAreaResized(int)), this, SLOT(_q_desktopWorkareaChanged()));
#else
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setAcceptedMouseButtons(Qt::LeftButton);
#endif

    d->rootWidget = QApplication::topLevelWidgets().value(0);
    Q_ASSERT(d->rootWidget);

    d->optionsAction = new QAction(d->rootWidget);
    d->optionsAction->setText(tr("Options"));
    d->rootWidget->addAction(d->optionsAction);
    connect(d->optionsAction, SIGNAL(triggered(bool)), this, SLOT(_q_optionsSelected()));

    d->exitAction = new QAction(d->rootWidget);
    d->exitAction->setText(tr("Exit"));
    d->rootWidget->addAction(d->exitAction);
    connect(d->exitAction, SIGNAL(triggered(bool)), this, SLOT(_q_exitSelected()));

    d->backAction = new QAction(d->rootWidget);
    d->backAction->setText(tr("Back"));
    d->rootWidget->addAction(d->backAction);
    connect(d->backAction, SIGNAL(triggered(bool)), this, SLOT(_q_backSelected()));

    d->updateFullScreen();
}

SDeclarativeWindowDecoration::~SDeclarativeWindowDecoration()
{
}

int SDeclarativeWindowDecoration::orientation() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->orientation;
}

void SDeclarativeWindowDecoration::setOrientation(int orientation)
{
    Q_D(SDeclarativeWindowDecoration);
    if (orientation != d->orientation) {
        d->orientation = orientation;
        emit orientationChanged(orientation);
#ifndef Q_OS_SYMBIAN
        // For desktop orientation change
        d->updateFullScreen();
#endif
    }
}

bool SDeclarativeWindowDecoration::isBackButtonVisible() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->backButtonVisible;
}

void SDeclarativeWindowDecoration::setBackButtonVisible(bool backButtonVisible)
{
    Q_D(SDeclarativeWindowDecoration);
    if (backButtonVisible != d->backButtonVisible) {
        d->backButtonVisible = backButtonVisible;
        emit backButtonVisibleChanged(backButtonVisible);
        d->updateCba();
    }
}

bool SDeclarativeWindowDecoration::isStatusBarVisible() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->statusBarVisible;
}

void SDeclarativeWindowDecoration::setStatusBarVisible(bool statusBarVisible)
{
    Q_D(SDeclarativeWindowDecoration);
    if (statusBarVisible != d->statusBarVisible) {
        d->statusBarVisible = statusBarVisible;
        emit statusBarVisibleChanged(statusBarVisible);
        d->updateFullScreen();
    }
}

bool SDeclarativeWindowDecoration::isTitleBarVisible() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->titleBarVisible;
}

void SDeclarativeWindowDecoration::setTitleBarVisible(bool titleBarVisible)
{
    Q_D(SDeclarativeWindowDecoration);
    if (titleBarVisible != d->titleBarVisible) {
        d->titleBarVisible = titleBarVisible;
        emit titleBarVisibleChanged(titleBarVisible);
        d->updateFullScreen();
    }
}

qreal SDeclarativeWindowDecoration::topDecorationHeight() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->topDecorationHeight;
}

void SDeclarativeWindowDecoration::setTopDecorationHeight(qreal topDecorationHeight)
{
    Q_D(SDeclarativeWindowDecoration);
    if (topDecorationHeight != d->topDecorationHeight) {
        d->topDecorationHeight = topDecorationHeight;
        emit topDecorationHeightChanged(topDecorationHeight);
    }
}

qreal SDeclarativeWindowDecoration::bottomDecorationHeight() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->bottomDecorationHeight;
}

void SDeclarativeWindowDecoration::setBottomDecorationHeight(qreal bottomDecorationHeight)
{
    Q_D(SDeclarativeWindowDecoration);
    if (bottomDecorationHeight != d->bottomDecorationHeight) {
        d->bottomDecorationHeight = bottomDecorationHeight;
        emit bottomDecorationHeightChanged(bottomDecorationHeight);
    }
}

QString SDeclarativeWindowDecoration::title() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->title;
}

void SDeclarativeWindowDecoration::setTitle(const QString &title)
{
    Q_D(SDeclarativeWindowDecoration);
    if (title != d->title) {
        d->title = title;
        d->rootWidget->setWindowTitle(d->title);
        emit titleChanged(title);
    }
}

QDeclarativeItem *SDeclarativeWindowDecoration::toolBar() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return d->toolBar;
}

void SDeclarativeWindowDecoration::setToolBar(QDeclarativeItem *toolBar)
{
    Q_D(SDeclarativeWindowDecoration);
    if (toolBar != d->toolBar) {
        d->toolBar = toolBar;
        emit toolBarChanged(toolBar);
    }
}

bool SDeclarativeWindowDecoration::isLandscape() const
{
    Q_D(const SDeclarativeWindowDecoration);
    return (d->orientation == SDeclarativeScreen::Landscape) || (d->orientation == SDeclarativeScreen::LandscapeInverted);
}

#ifndef Q_OS_SYMBIAN
void SDeclarativeWindowDecoration::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(SDeclarativeWindowDecoration);
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // show hard-coded decorators for desktop purposes
    painter->save();

    // CBA
    const qreal cbaHeight = bottomDecorationHeight();
    if (cbaHeight > 1) {

        painter->drawRect(QRect(0, height() - cbaHeight, width(), cbaHeight));

        QFont cbaFont;
        cbaFont.setPixelSize(cbaHeight / 2);
        painter->setFont(cbaFont);

        QAction *left = d->leftSoftkey();
        QAction *right = d->rightSoftkey();

        if (left)
            painter->drawText(QRectF(0, height()-cbaHeight, width() / 2, cbaHeight), left->text(), QTextOption(Qt::AlignCenter));

        if (right)
            painter->drawText(QRectF(width() / 2, height()-cbaHeight, width() / 2, cbaHeight), right->text(), QTextOption(Qt::AlignCenter));
    }

    // Status/title pane
    const qreal statusPaneHeight = topDecorationHeight();
    if (statusPaneHeight > 1) {

        QFont spFont;
        spFont.setBold(true);
        spFont.setPixelSize(statusPaneHeight / 3);
        painter->setFont(spFont);

        const QRect statusPaneRect(0,0,width(), statusPaneHeight);
        painter->drawRect(statusPaneRect);

        if (d->titleBarVisible) {
            QString titleText = d->title;
            if (!titleText.length()) {
                QFileInfo info(QCoreApplication::applicationFilePath());
                titleText = info.baseName();
            }

            painter->drawText(statusPaneRect, titleText, QTextOption(Qt::AlignCenter));
        }
    }

    painter->restore();
}

void SDeclarativeWindowDecoration::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(SDeclarativeWindowDecoration);
    // check hit on CBA
    const qreal cbaHeight = bottomDecorationHeight();
    if (event->pos().y() > height() - cbaHeight) {
        // within the CBA
        if (event->pos().x() < width() / 2) {
            QAction *left = d->leftSoftkey();
            if (left)
                left->trigger();
        } else {
            QAction *right = d->rightSoftkey();
            if (right)
                right->trigger();
        }
        event->accept();
    } else {
        event->ignore();
    }
}
#endif // !Q_OS_SYMBIAN

#include "moc_sdeclarativewindowdecoration.cpp"
