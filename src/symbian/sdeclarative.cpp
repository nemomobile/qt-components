/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "sdeclarative.h"
#include "siconpool.h"
#include <QCoreApplication>
#include <QTime>
#include <QTimer>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QPixmapCache>
#ifdef Q_OS_SYMBIAN
#include <e32std.h>
#include <AknUtils.h>
#if defined(HAVE_SYMBIAN_INTERNAL)
#include <aknsmallindicator.h>
#endif // HAVE_SYMBIAN_INTERNAL
#include <aknappui.h>
#include <avkon.rsg>
#endif // Q_OS_SYMBIAN

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#endif

//#define Q_DEBUG_SDECLARATIVE
#if defined(Q_DEBUG_SDECLARATIVE)
#include <QDebug>
#endif // Q_DEBUG_SDECLARATIVE

static const int MINUTE_MS = 60*1000;

#ifdef Q_OS_SYMBIAN
_LIT(KTimeFormat, "%J%:1%T");
#endif

class SDeclarativePrivate
{
public:
    SDeclarativePrivate() :
        mListInteractionMode(SDeclarative::TouchInteraction), foreground(true) {}

    int allocatedMemory() const;

    SDeclarative::InteractionMode mListInteractionMode;
    QTimer timer;
    bool foreground;
};

int SDeclarativePrivate::allocatedMemory() const
{
#if defined(Q_OS_SYMBIAN)
    TInt totalAllocated;
    User::AllocSize(totalAllocated);
    return totalAllocated;
#elif defined(Q_OS_WIN)
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
        return -1;
    return pmc.WorkingSetSize;
#else
    return -1;
#endif
}

SDeclarative::SDeclarative(QObject *parent) :
    QObject(parent),
    d_ptr(new SDeclarativePrivate)
{
    d_ptr->timer.start(MINUTE_MS);
    connect(&d_ptr->timer, SIGNAL(timeout()), this, SIGNAL(currentTimeChanged()));
    QCoreApplication::instance()->installEventFilter(this);
}

SDeclarative::~SDeclarative()
{
    d_ptr->timer.stop();
}

SDeclarative::InteractionMode SDeclarative::listInteractionMode() const
{
    return d_ptr->mListInteractionMode;
}

void SDeclarative::setListInteractionMode(SDeclarative::InteractionMode mode)
{
    if (d_ptr->mListInteractionMode != mode) {
        d_ptr->mListInteractionMode = mode;
        emit listInteractionModeChanged();
    }
}

QString SDeclarative::currentTime()
{
#ifdef Q_OS_SYMBIAN
    TBuf<15> time;
    TTime homeTime;
    homeTime.HomeTime();
    TRAP_IGNORE(homeTime.FormatL(time, KTimeFormat));
    // Do the possible arabic indic digit etc. conversions
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion(time);
    return QString(reinterpret_cast<const QChar *>(time.Ptr()), time.Length());
#else
    return QTime::currentTime().toString(QLatin1String("h:mm"));
#endif
}

#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
static void DoShowIndicatorPopupL()
{
    MEikAppUiFactory *factory = CEikonEnv::Static()->AppUiFactory();
    if (factory) {
        CEikStatusPane* statusPane = factory->StatusPane();
        if (statusPane && statusPane->CurrentLayoutResId() != R_AVKON_WIDESCREEN_PANE_LAYOUT_IDLE_FLAT_NO_SOFTKEYS) {
            // statusPane SwitchLayoutL is needed for positioning Avkon indicator popup (opened from StatusBar) correctly
            statusPane->SwitchLayoutL(R_AVKON_WIDESCREEN_PANE_LAYOUT_IDLE_FLAT_NO_SOFTKEYS);
        }
    }
    CAknSmallIndicator* indicator = CAknSmallIndicator::NewLC(TUid::Uid(0));
    indicator->HandleIndicatorTapL();
    CleanupStack::PopAndDestroy(indicator);
}
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL

int SDeclarative::privateAllocatedMemory() const
{
    return d_ptr->allocatedMemory();
}

void SDeclarative::privateClearIconCaches()
{
    SIconPool::releaseAll();
    QPixmapCache::clear();
}

void SDeclarative::privateClearComponentCache()
{
    QDeclarativeContext *context = qobject_cast<QDeclarativeContext*>(this->parent());
    if (context)
        context->engine()->clearComponentCache();
}

void SDeclarative::privateShowIndicatorPopup()
{
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    QT_TRAP_THROWING(DoShowIndicatorPopupL());
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL
}

bool SDeclarative::isForeground()
{
    return d_ptr->foreground;
}

bool SDeclarative::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == QCoreApplication::instance()) {
        if (event->type() == QEvent::ApplicationActivate) {
            emit currentTimeChanged();
            d_ptr->timer.start(MINUTE_MS);
            d_ptr->foreground = true;
            emit foregroundChanged();
        } else if (event->type() == QEvent::ApplicationDeactivate) {
            d_ptr->timer.stop();
            d_ptr->foreground = false;
            emit foregroundChanged();
        }
    }
    return QObject::eventFilter(obj, event);
}
