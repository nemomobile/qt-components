/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
#include "sdeclarativesharedstatusbar.h"
#include "sdeclarativesharedstatusbar_p_symbian.h"
#include "sdeclarativescreen.h"
#include "sdeclarativeincallindicator.h"
#include <AknUtils.h>
#include <akniconconfig.h>
#include <aknlayoutscalable_avkon2.cdl.h>
#include <AknsDrawUtils.h>
#include <QPainter>
#include <QApplication>

#ifdef Q_DEBUG_SHARED_STATUSBAR
#include <QDebug>
#endif // Q_DEBUG_SHARED_STATUSBAR

SDeclarativeSharedStatusBarPrivate::SDeclarativeSharedStatusBarPrivate(SDeclarativeSharedStatusBar *qq)
: q_ptr(qq), useExtendedApi(false)
{
    QT_TRAP_THROWING(
            bitmapSubscriber.reset( CSharedBitmapSubcriber::NewL( *this ) );
            statusPaneSubscriber.reset( CSDeclarativeStatusPaneSubscriber::NewL( *this ) );
            incallIndicator.reset( CSDeclarativeIncallIndicator::NewL() );
    );

    TInt layoutInfoDummy;
    if (RProperty::Get(KOffScreenSPaneUid, KOffScreenLayoutInfo, layoutInfoDummy) == KErrNone)
        useExtendedApi = true;

    // request redraw. ignore error.
    RProperty::Set(KOffScreenSPaneUid, KOffScreenUpdater, TInt());

    const TAknIndicatorState& indicatorState = statusPaneSubscriber->IndicatorState();
    incallIndicator->SetFlags( indicatorState.iIncallBubbleFlags );
}

SDeclarativeSharedStatusBarPrivate::~SDeclarativeSharedStatusBarPrivate()
{
}

void SDeclarativeSharedStatusBarPrivate::reset()
{
#ifdef Q_DEBUG_SHARED_STATUSBAR
    qDebug() << "SDeclarativeSharedStatusBarPrivate::reset()";
#endif
    Q_Q(SDeclarativeSharedStatusBar);
    foregroundPixmap = QPixmap();
    backgroundPixmap = QPixmap();
    q->update();
}

QPixmap &SDeclarativeSharedStatusBarPrivate::symbianForegroundPixmapL()
{
    if (foregroundPixmap.isNull()) {
        CFbsBitmap* bitmap = 0;
        if (useExtendedApi) {
            TBool isMirrored;
            bitmap = bitmapSubscriber->CreateSharedBitmapL(&isMirrored);
            mirroredBitmap = bool(isMirrored);
        } else {
            bitmap = bitmapSubscriber->CreateSharedBitmapL();
            mirroredBitmap = (bool)AknLayoutUtils::LayoutMirrored();
        }

        foregroundPixmap = QPixmap::fromSymbianCFbsBitmap(bitmap);
        delete bitmap;

#ifdef Q_DEBUG_SHARED_STATUSBAR
        qDebug() << "SDeclarativeSharedStatusBarPrivate::symbianForegroundPixmapL() got new bitmap of size: " << foregroundPixmap.width() << "x" << foregroundPixmap.height() ;
#endif
    }
    return foregroundPixmap;
}

QPixmap &SDeclarativeSharedStatusBarPrivate::symbianBackgroundPixmapL()
{
    Q_Q(SDeclarativeSharedStatusBar);
    if (backgroundPixmap.isNull()) {

        // get sizes
        TAknLayoutRect layoutRect;
        layoutRect.LayoutRect( TRect(0, 0, TInt(q->width()), TInt(q->height())), AknLayoutScalable_Avkon2::status_pane( 0 ).LayoutLine() );
        TRect backgroundOuterRect = layoutRect.Rect();

        layoutRect.LayoutRect( backgroundOuterRect, AknLayoutScalable_Avkon2::bg_pane_g2().LayoutLine() );
        TRect backgroundInnerRect = layoutRect.Rect();

        // Create a bitmap to be used off-screen.
        CFbsBitmap* offScreenBitmap = new (ELeave) CFbsBitmap();
        CleanupStack::PushL(offScreenBitmap);
        AknIconConfig::TPreferredDisplayMode mode;
        AknIconConfig::PreferredDisplayMode(mode, AknIconConfig::EImageTypeOffscreen);
        User::LeaveIfError(offScreenBitmap->Create(TSize(backgroundOuterRect.Size()),mode.iBitmapMode));

        // Create an off-screen device and context.
        CFbsBitGc* bitmapContext=NULL;
        CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(offScreenBitmap);
        CleanupStack::PushL(bitmapDevice);
        User::LeaveIfError(bitmapDevice->CreateContext(bitmapContext));

        // Draw on the bitmap.
        TBool wasDrawn = AknsDrawUtils::DrawFrame(
            AknsUtils::SkinInstance(),
            *bitmapContext,
            backgroundOuterRect,
            backgroundInnerRect,
            KAknsIIDQtgFrStatusbarNormal,
            KAknsIIDDefault );

        backgroundPixmap = QPixmap::fromSymbianCFbsBitmap(offScreenBitmap);

        delete bitmapContext;
        CleanupStack::PopAndDestroy(bitmapDevice);
        CleanupStack::PopAndDestroy(offScreenBitmap); 
        
    }
    return backgroundPixmap;
}

void SDeclarativeSharedStatusBarPrivate::SharedBitmapChanged()
{
    reset();
}

void SDeclarativeSharedStatusBarPrivate::setOrientation(int orientation)
{
    TInt layoutInfo(0);
    if (RProperty::Get(KOffScreenSPaneUid, KOffScreenLayoutInfo, layoutInfo) == KErrNone) {
        bool applicationInLandscape = (orientation&(SDeclarativeScreen::Landscape|SDeclarativeScreen::LandscapeInverted));
        bool bitmapInLandscape = (layoutInfo&ELayoutInfoLandscapeFlag);

#ifdef Q_DEBUG_SHARED_STATUSBAR
        qDebug() << "SDeclarativeSharedStatusBarPrivate::setOrientation() app in lscp: " << applicationInLandscape << " bitmap in lscp:" << bitmapInLandscape;
#endif

        if (applicationInLandscape != bitmapInLandscape) {
            // need to update the bitmap layout
            TInt newLayoutInfo = layoutInfo&ELayoutInfoMirroredFlag;
            if (applicationInLandscape)
                newLayoutInfo |= ELayoutInfoLandscapeFlag;

#ifdef Q_DEBUG_SHARED_STATUSBAR
            qDebug() << "SDeclarativeSharedStatusBarPrivate::setOrientation() setting new layout info: " << (int)newLayoutInfo;
#endif

            // set the layout info
            TInt err = RProperty::Set(KOffScreenSPaneUid, KOffScreenLayoutSwitch, newLayoutInfo);
#ifdef Q_DEBUG_SHARED_STATUSBAR
            if (err != KErrNone)
                qDebug() << "SDeclarativeSharedStatusBarPrivate::setOrientation() setting new layout info error: " << (int)err;
#endif
            qt_symbian_throwIfError((int)err);
        }
    }
#ifdef Q_DEBUG_SHARED_STATUSBAR
    else if (useExtendedApi){
        qDebug() << "SDeclarativeSharedStatusBarPrivate::setOrientation() - error reading KOffScreenLayoutInfo";
    }
#endif

    // request redraw. ignore error.
    RProperty::Set(KOffScreenSPaneUid, KOffScreenUpdater, TInt());
    reset();
}

void SDeclarativeSharedStatusBarPrivate::setForeground(bool foreground)
{
#ifdef Q_DEBUG_SHARED_STATUSBAR
    qDebug() << "SDeclarativeSharedStatusBarPrivate::setForeground() foreground: " << foreground;
#endif
    if (!foreground)
        bitmapSubscriber->Cancel();
    else
        reset();
}

void SDeclarativeSharedStatusBarPrivate::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
#ifdef Q_DEBUG_SHARED_STATUSBAR
    qDebug() << "SDeclarativeSharedStatusBarPrivate::geometryChanged() old: " << oldGeometry << " new: " << newGeometry;
#endif
    if (newGeometry.size() != oldGeometry.size()) {
        reset();
    }
}

void SDeclarativeSharedStatusBarPrivate::paint(QPainter *painter)
{
    Q_Q(SDeclarativeSharedStatusBar);

    // draw the background frame
    QPixmap drawnBackgroundPixmap;
    QT_TRAP_THROWING(drawnBackgroundPixmap = symbianBackgroundPixmapL());
    if (drawnBackgroundPixmap.isNull()) {
        // fallback: draw black
        painter->setBrush(Qt::black);
        painter->drawRect(0, 0, q->width(), q->height());
    } else {
        painter->drawPixmap(QPoint(), drawnBackgroundPixmap);
    }

    // draw the foreground data
    QPixmap drawnForegroundPixmap;
    QT_TRAP_THROWING(drawnForegroundPixmap = symbianForegroundPixmapL());

#ifdef Q_DEBUG_SHARED_STATUSBAR
    if (drawnForegroundPixmap.isNull())
        qDebug() << "SDeclarativeSharedStatusBar::paint() foreground pixmap NULL!";
#endif

    if (mirroredBitmap)
        painter->drawPixmap(QRect(0, 0, q->width(), q->height()), drawnForegroundPixmap, QRect(0, 0, q->width(), q->height()));
    else
        painter->drawPixmap(QRect(0, 0, q->width(), q->height()), drawnForegroundPixmap, QRect(drawnForegroundPixmap.width() - q->width(), 0, q->width(), q->height()));

#ifdef Q_DEBUG_SHARED_STATUSBAR
    qDebug() << "SDeclarativeSharedStatusBar::paint() statusbar size: " << q->width() << "x" << q->height() << " bitmap size: " << drawnForegroundPixmap.width() << "x" << drawnForegroundPixmap.height() << " RtL: " << mirroredBitmap;
#endif
}

void SDeclarativeSharedStatusBarPrivate::StatusPaneStateChanged( TStatusPaneChangeFlags aChangeFlags )
    {
    if ( aChangeFlags&MSDeclarativeStatusPaneSubscriberObverver::EStatusPaneIndicatorsState )
        {
        const TAknIndicatorState& indicatorState = statusPaneSubscriber->IndicatorState();
        incallIndicator->SetFlags( indicatorState.iIncallBubbleFlags );
        }
    }
