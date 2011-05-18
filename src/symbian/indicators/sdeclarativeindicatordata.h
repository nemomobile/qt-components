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

#ifndef SDECLARATIVEINDICATORDATA_H
#define SDECLARATIVEINDICATORDATA_H

#include <e32base.h> // CBase

class TResourceReader;

class CSDeclarativeIndicatorData : public CBase
{
public:
    static CSDeclarativeIndicatorData* NewL( TResourceReader& aReader );
    ~CSDeclarativeIndicatorData();

    TInt Uid() const;
    TInt Priority() const;
    TInt FrameCount() const;
    TPtrC BitmapFile() const;
    TBool MultiColorMode() const;
    TInt GetIndicatorBitmapIndexes( RArray<TInt>& aIndexes ) const;

    TInt State() const;
    void SetState( TInt aState );
    TInt CurrentFrame() const;
    void SetCurrentFrame( TInt aCurrentFrame );

private:
    CSDeclarativeIndicatorData();
    void ConstructFromResourceL( TResourceReader& aReader );
    void ReadBitmapIndexesL( TResourceReader& aReader, TInt aCount );

private: // data
    TInt iState;
    TInt iUid;
    TInt iPriority;
    TInt iFrameCount;
    TInt iCurrentFrame;
    HBufC* iBitmapFile;
    TBool iMultiColorMode;
    RArray<TInt> iIndicatorBitmapIndexes;

};

#endif // SDECLARATIVEINDICATORDATA_H
