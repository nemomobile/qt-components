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

#include "sdeclarativeimageprovider.h"
#include "siconpool.h"
#include "sdeclarative.h"

#include <QLibrary>

class SDeclarativeImageProviderPrivate
{
    Q_DECLARE_PUBLIC(SDeclarativeImageProvider)
public:
    SDeclarativeImageProviderPrivate(SDeclarativeImageProvider *qq);
    SDeclarativeImageProvider *q_ptr;
    QDeclarativeImageProvider *sharedImageProvider;
};

SDeclarativeImageProviderPrivate::SDeclarativeImageProviderPrivate(SDeclarativeImageProvider *qq)
    : q_ptr(qq),
      sharedImageProvider(0)
{
    // try to load shared image provider
    typedef QDeclarativeImageProvider* (*CreateImageProviderFuncPtr)();
    CreateImageProviderFuncPtr createSharedImageProvider =
            (CreateImageProviderFuncPtr) QLibrary::resolve("qtuisharedimageprovider", "1");

    if (createSharedImageProvider)
        sharedImageProvider = createSharedImageProvider();
}

SDeclarativeImageProvider::SDeclarativeImageProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
    d_ptr(new SDeclarativeImageProviderPrivate(this))
{
}

SDeclarativeImageProvider::~SDeclarativeImageProvider()
{
}

QPixmap SDeclarativeImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap pixmap;
    if (!id.isEmpty() && requestedSize.width() && requestedSize.height()) {
        Q_D(SDeclarativeImageProvider);

        // first try from shared image provider and then from in-process image provider
        if (d->sharedImageProvider)
            pixmap = d->sharedImageProvider->requestPixmap(SDeclarative::resolveIconFileName(id), size, requestedSize);

        if (pixmap.isNull())
            pixmap = SIconPool::get(SDeclarative::resolveIconFileName(id), requestedSize, Qt::KeepAspectRatio);

        if (!pixmap.isNull() && size)
            *size = pixmap.size();
    }
    return pixmap;
}

bool SDeclarativeImageProvider::graphicsSharing() const
{
    Q_D(const SDeclarativeImageProvider);
    return d->sharedImageProvider;
}
