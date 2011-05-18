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

#include "spopupmanager.h"

#ifdef Q_OS_SYMBIAN
#include <aknappui.h>
#include <avkon.rsg>
#if defined(HAVE_SYMBIAN_INTERNAL)
#include <aknsmallindicator.h>
#endif // HAVE_SYMBIAN_INTERNAL
#endif // Q_OS_SYMBIAN

//#define Q_DEBUG_SPOPUPMANAGER
#if defined(Q_DEBUG_SPOPUPMANAGER)
#include <QDebug>
#endif // Q_DEBUG_SPOPUPMANAGER

class SPopupManagerPrivate
{
public:
    SPopupManagerPrivate() : popupStackDepth(0) {}

    int popupStackDepth;
};

SPopupManager::SPopupManager(QObject *parent) :
    QObject(parent),
    d_ptr(new SPopupManagerPrivate)
{
}

SPopupManager::~SPopupManager()
{
}

int SPopupManager::popupStackDepth() const
{
    return d_ptr->popupStackDepth;
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

void SPopupManager::privateShowIndicatorPopup()
{
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    QT_TRAP_THROWING(DoShowIndicatorPopupL());
#endif // Q_OS_SYMBIAN && HAVE_SYMBIAN_INTERNAL
}

void SPopupManager::privateNotifyPopupOpen()
{
    d_ptr->popupStackDepth++;
    emit popupStackDepthChanged();
}

void SPopupManager::privateNotifyPopupClose()
{
    if (d_ptr->popupStackDepth > 0) {
        d_ptr->popupStackDepth--;
        emit popupStackDepthChanged();
    }
}
