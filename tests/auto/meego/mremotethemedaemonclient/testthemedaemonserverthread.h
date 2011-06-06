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

