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

#ifndef MTESTTHEMEDAEMONSERVERTHREAD_H
#define MTESTTHEMEDAEMONSERVERTHREAD_H

#include <QHash>
#include <QString>
#include <QThread>

#include <themedaemon/mthemedaemonprotocol.h>

class QPixmap;

/**
 * \brief Thread that runs the test-themedaemon-server.
 *
 * Used for testing the MThemeDaemonClient which uses sockets to
 * communicate with the themedaemon-server.
 */
class TestThemeDaemonServerThread : public QThread
{
    Q_OBJECT

public:
    /**
     * \param serverAddress Address of the server where the client will be connected to.
     * \param parent        Parent object.
     */
    TestThemeDaemonServerThread(const QString &serverAddress, QObject *parent = 0);

    virtual ~TestThemeDaemonServerThread();

    /**
     * \return The number of cached pixmaps after the themedaemon-server has send
     *         the most used pixmaps. Is a helper method for the unit tests to verify
     *         whether the themedaemon client has cached the most used pixmaps.
     */
    int cachedMostUsedPixmapsCount() const;

protected:
    virtual void run();

private:
    /**
     * Creates a pixmap with the ID \a imageId and the size \a size and adds
     * it to m_pixmapCache.
     */
    void createPixmap(const QString &imageId, const QSize &size = QSize());

    QString m_serverAddress;
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, const QPixmap*> m_pixmapCache;
};

#endif

