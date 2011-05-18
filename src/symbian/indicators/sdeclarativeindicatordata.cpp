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

#include "sdeclarativeindicatordata.h"

#include <aknconsts.h> // KAvkonBitmapFile
#include <barsread.h> // TResourceReader

CSDeclarativeIndicatorData* CSDeclarativeIndicatorData::NewL( TResourceReader& aReader )
    {
    CSDeclarativeIndicatorData* self = new (ELeave) CSDeclarativeIndicatorData();
    CleanupStack::PushL( self );
    self->ConstructFromResourceL( aReader );
    CleanupStack::Pop( self );
    return self;
    }

CSDeclarativeIndicatorData::CSDeclarativeIndicatorData() :
    iState( EAknIndicatorStateOff ),
    iFrameCount( 1 )
    {
    }

CSDeclarativeIndicatorData::~CSDeclarativeIndicatorData()
    {
    iIndicatorBitmapIndexes.Close();
    delete iBitmapFile;
    }

void CSDeclarativeIndicatorData::ConstructFromResourceL( TResourceReader& aReader )
    {
    iUid = aReader.ReadInt16();

    iMultiColorMode = (iUid == EAknIndicatorVtRecord);

    iPriority = aReader.ReadInt16(); // narrow priority
    aReader.ReadInt16(); // ignore the wide priority
    iBitmapFile = aReader.ReadHBufCL(); // bmp filename

    // If default icon file is used we delete filename to save some memory
   if ( iBitmapFile && !iBitmapFile->CompareF( KAvkonBitmapFile ) )
        {
        delete iBitmapFile;
        iBitmapFile = NULL;
        }

    TInt count = aReader.ReadInt16();  // Number of states

    for ( TInt i = 0; i < count; ++i )
        {
        TInt stateId = aReader.ReadInt16();
        if ( stateId != EAknIndicatorStateOff )
            {
            TInt iconCount = aReader.ReadInt16();
            if ( stateId == EAknIndicatorStateAnimate )
                iFrameCount = iconCount;

            ReadBitmapIndexesL( aReader, iconCount );
            }
        }
    }

TInt CSDeclarativeIndicatorData::Uid() const
    {
    return iUid;
    }

TInt CSDeclarativeIndicatorData::Priority() const
    {
    return iPriority;
    }

TInt CSDeclarativeIndicatorData::FrameCount() const
    {
    return iFrameCount;
    }

TBool CSDeclarativeIndicatorData::MultiColorMode() const
    {
    return iMultiColorMode;
    }

TPtrC CSDeclarativeIndicatorData::BitmapFile() const
    {
    if ( iBitmapFile )
        return iBitmapFile->Des();

    return KNullDesC();
    }

TInt CSDeclarativeIndicatorData::State() const
    {
    return iState;
    }

void CSDeclarativeIndicatorData::SetState( TInt aState )
    {
    iState = aState;
    }

TInt CSDeclarativeIndicatorData::CurrentFrame() const
    {
    return iCurrentFrame;
    }

void CSDeclarativeIndicatorData::SetCurrentFrame( TInt aCurrentFrame )
    {
    iCurrentFrame = aCurrentFrame;
    }

TInt CSDeclarativeIndicatorData::GetIndicatorBitmapIndexes( RArray<TInt>& aIndexes ) const
    {
    TInt err = KErrNone;
    for ( TInt i = 0; i < iIndicatorBitmapIndexes.Count() && err == KErrNone; ++i )
        {
        err = aIndexes.Append( iIndicatorBitmapIndexes[i] );
        }
    return err;
    }

void CSDeclarativeIndicatorData::ReadBitmapIndexesL( TResourceReader& aReader, TInt aCount )
    {
    for ( TInt i = 0; i < aCount; ++i )
        {
        TInt bitmapId = aReader.ReadInt16(); // narrow bitmap id
        TInt maskId = aReader.ReadInt16(); // narrow mask id

        if ( bitmapId == KErrNotFound ) // no bitmap
            {
            bitmapId = 0;
            maskId = 0;
            }
        else if ( maskId == KErrNotFound ) // no mask, just bitmap
            {
            maskId = bitmapId;
            }

        User::LeaveIfError( iIndicatorBitmapIndexes.Append( bitmapId ) );
        User::LeaveIfError( iIndicatorBitmapIndexes.Append( maskId ) );


        // ignore wide bitmaps
        aReader.ReadInt16(); // wide bitmap id
        aReader.ReadInt16(); // wide mask id
        }
    }

//  End of File
