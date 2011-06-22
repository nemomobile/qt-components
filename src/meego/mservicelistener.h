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

#ifndef MSERVICELISTENER_H
#define MSERVICELISTENER_H

#include <QDBusConnection>
#include <QDBusServiceWatcher>

class QDBusPendingCallWatcher;

/*! \class MServiceListener

  \note This is a copy of libmeegotouch's MServiceListener as it is not public class
  and the same functionality is needed here to detect disappearance of the
  'RemoteScreen.TopEdge' property so that we don't end up with screen locked to some
  orientation indefinitely.

  \brief Extends the QDBusServiceWatcher; adds the possibility to do an
  initial check of the service status (asyncronously) and to query the status
  of the service any time.

  When you create an instance of this class, it won't open any D-Bus
  connections. When startListening is called, the instance starts using the
  QDBusServiceWatcher to get updates when the service is registered or
  unregistered.  It can also check the current status of the service by
  executing an asynchronous NameHasOwner call.

  If the specified service appears on D-Bus, it will emit the \c
  nameAppeared() signal, if disappears, then the nameDisappeared()
  signal.  An initial query and signal emission will be done if \c
  initialCheck is true, which is the default.

  Anytime you can check with <tt>isServicePresent()</tt> if according
  to our current knowledge (last signal emission) the service is
  present or not.  This means that if \c initialCheck is false,
  <tt>isServicePresent()</tt> can return false, even though the service
  is present.
*/
class MServiceListener : public QDBusServiceWatcher
{
    Q_OBJECT
public:
    enum ServicePresence {NotPresent = 0, Present, Unknown};

    explicit MServiceListener(QDBusConnection::BusType busType, const QString &busName, QObject *parent = 0);
    explicit MServiceListener(const QDBusConnection bus, const QString &busName, QObject *parent = 0);
    virtual ~MServiceListener();

    /*
      Start listening to the service registration / unregistration over
      D-Bus. If \a nameHasOwnerCheck is true, also send a NameHasOwner query to
      D-Bus (asyncronously).
     */
    void startListening(bool nameHasOwnerCheck);

    /*
      Return our current understanding (not present, present, unknown)
      of the presence of the watched service.
     */
    ServicePresence isServicePresent() const;

private Q_SLOTS:
    /*
      Handling of the asynchronous reply of the initial query.
     */
    void onNameHasOwnerFinished(QDBusPendingCallWatcher *watcher);
    void setServicePresent();
    void setServiceGone();

private:
    /*
      Initializes the QDBusServiceWatcher base part and connects the signals
      from it.
    */
    void init();
    ServicePresence servicePresent; ///< Our current understanding about the service name's state
    QDBusConnection::BusType busType; ///< The service bus type we are interested in
    QString busName; ///< The service name we are interested in
    bool listeningStarted;
    QDBusConnection *connection;

Q_SIGNALS:
    void nameAppeared();
    void nameDisappeared();
};

#endif // MSERVICELISTENER_H
