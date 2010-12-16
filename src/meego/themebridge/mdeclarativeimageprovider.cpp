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

#include "mdeclarativeimageprovider.h"

#include <themedaemon/mlocalthemedaemonclient.h>
#include <themedaemon/mremotethemedaemonclient.h>

MDeclarativeImageProvider::MDeclarativeImageProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap),
    m_themeDaemonClient(0)
{
    MRemoteThemeDaemonClient *remoteThemeDaemonClient = new MRemoteThemeDaemonClient();
    if (remoteThemeDaemonClient->isConnected()) {
        m_themeDaemonClient = remoteThemeDaemonClient;
    } else {
        delete remoteThemeDaemonClient;
        m_themeDaemonClient = new MLocalThemeDaemonClient();
    }
}

MDeclarativeImageProvider::~MDeclarativeImageProvider()
{
    delete m_themeDaemonClient;
}

QPixmap MDeclarativeImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    const QPixmap pixmap = m_themeDaemonClient->requestPixmap(id, requestedSize);
    if (!pixmap.isNull() && size) {
        *size = pixmap.size();
    }

    return pixmap;
}
