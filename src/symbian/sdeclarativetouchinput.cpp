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
#include "sdeclarativetouchinput.h"
#include <e32std.h>

#if defined(HAVE_SYMBIAN_INTERNAL)
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#endif //HAVE_SYMBIAN_INTERNAL

const TUint32 KAknFepTouchInputActive = 0x00000004;
const TUint32 KAknFepSoftwareInputpanelHeight = 0x00000005;
const TUid KPSUidAknFep = { 0x100056de };

CTouchInput* CTouchInput::NewL( MTouchInputStateObserver& aObserver )
    {
    CTouchInput* self = new ( ELeave ) CTouchInput( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CTouchInput::CTouchInput( MTouchInputStateObserver& aObserver )
    : CActive( CActive::EPriorityStandard )
    , iObserver( aObserver )
    , iVisible( EFalse )
    , iPortraitHeight( KErrNotReady )
    , iLandscapeHeight( KErrNotReady )
    {
    }

void CTouchInput::ConstructL()
    {
    CActiveScheduler::Add( this );
    User::LeaveIfError( iTouchInputState.Attach( KPSUidAknFep, KAknFepTouchInputActive ) );
    iHeightWatcher = CHeightWatcher::NewL( *this, iObserver );
    Subscribe();
    }

CTouchInput::~CTouchInput()
    {
    Cancel();
    iTouchInputState.Close();
    delete iHeightWatcher;
    }

TBool CTouchInput::Visible() const
    {
    return iVisible;
    }

TReal CTouchInput::PortraitHeight()
    {
    iPortraitHeight = ProposedHeight();

    if ( iPortraitHeight != KErrNotReady )
        {
        return iPortraitHeight;
        }

#if defined(HAVE_SYMBIAN_INTERNAL)
    TRect screenRect;
    TAknLayoutRect portraitRect;
    TAknLayoutRect indicatorRect;

    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, screenRect );
    portraitRect.LayoutRect( screenRect, AknLayoutScalable_Avkon::popup_fep_ituss_window( 0 ).LayoutLine() );
    indicatorRect.LayoutRect( screenRect, AknLayoutScalable_Avkon::icf_edit_indi_pane( 2 ).LayoutLine() );
    iPortraitHeight = portraitRect.Rect().Height() + indicatorRect.Rect().Height();
#endif //HAVE_SYMBIAN_INTERNAL

    return iPortraitHeight;
    }

TReal CTouchInput::LandscapeHeight()
    {
    iLandscapeHeight = ProposedHeight();

    if ( iLandscapeHeight != KErrNotReady )
        {
        return iLandscapeHeight;
        }

#if defined(HAVE_SYMBIAN_INTERNAL)
    TRect screenRect;
    TAknLayoutRect landscapeRect;
    TAknLayoutRect indicatorRect;

    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, screenRect);
    landscapeRect.LayoutRect( screenRect, AknLayoutScalable_Avkon::popup_fep_vkbss_window( 0 ).LayoutLine() );
    indicatorRect.LayoutRect( screenRect, AknLayoutScalable_Avkon::icf_edit_indi_pane( 2 ).LayoutLine() );
    iLandscapeHeight = landscapeRect.Rect().Height() + indicatorRect.Rect().Height();
#endif //HAVE_SYMBIAN_INTERNAL

    return iLandscapeHeight;
    }

void CTouchInput::RunL()
    {
    Subscribe();
    TInt value = 0;
    User::LeaveIfError( iTouchInputState.Get( value ) );
    TBool temp = static_cast<TBool>( value );
    if ( temp != iVisible )
        {
        iVisible = temp;
        iObserver.VisibleChanged();
        }
    }

void CTouchInput::DoCancel()
    {
    iTouchInputState.Cancel();
    }

void CTouchInput::Subscribe()
    {
    if (!IsActive())
        {
        iTouchInputState.Subscribe(iStatus);
        SetActive();
        }
    }

TInt CTouchInput::ProposedHeight()
    {
    return iHeightWatcher->ProposedHeight();
    }


CHeightWatcher* CHeightWatcher::NewL( CTouchInput& aTouchInput, MTouchInputStateObserver& aObserver )
    {
    CHeightWatcher* self = new ( ELeave ) CHeightWatcher( aTouchInput, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CHeightWatcher::CHeightWatcher( CTouchInput& aTouchInput, MTouchInputStateObserver& aObserver )
    : CActive( EPriorityLow )
    , iTouchInput( aTouchInput )
    , iObserver ( aObserver )
    , iNotifiedHeight( KErrNotReady )
    {
    }

void CHeightWatcher::ConstructL()
    {
    CActiveScheduler::Add( this );
    User::LeaveIfError( iProposedHeight.Attach( KPSUidAknFep, KAknFepSoftwareInputpanelHeight ) );
    Subscribe();
    }

CHeightWatcher::~CHeightWatcher()
    {
    Cancel();
    iProposedHeight.Close();
    }

TInt CHeightWatcher::ProposedHeight()
    {
    TInt value = KErrNotReady;
    iProposedHeight.Get( value );
    value = value == 0 ? KErrNotReady : value;
    iNotifiedHeight = value;
    return value;
    }

void CHeightWatcher::RunL()
    {
    // Cannot leave so no need to implement RunError method; if error, do nothing.
    if ( iStatus.Int() == KErrNone )
        {
        Subscribe();
        TInt newHeight = KErrNotReady;
        TInt err = iProposedHeight.Get( newHeight );
        if ( !err && newHeight && newHeight != iNotifiedHeight && iTouchInput.Visible() )
            {
            iNotifiedHeight = newHeight;
            iObserver.HeightChanged();
            }
        }
    }

void CHeightWatcher::DoCancel()
    {
    iProposedHeight.Cancel();
    }

void CHeightWatcher::Subscribe()
    {
    if ( !IsActive() )
        {
        iProposedHeight.Subscribe(iStatus);
        SetActive();
        }
    }
