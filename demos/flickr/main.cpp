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

//![0]
#include <QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QNetworkProxy>
#include <QUrl>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QUrl proxyUrl(qgetenv("http_proxy"));
    if (proxyUrl.isValid() && !proxyUrl.host().isEmpty()) {
        int proxyPort = (proxyUrl.port() > 0) ? proxyUrl.port() : 8080;
        QNetworkProxy proxy(QNetworkProxy::HttpProxy, proxyUrl.host(), proxyPort);
        QNetworkProxy::setApplicationProxy(proxy);
    }
    else {
        QNetworkProxyQuery query(QUrl(QLatin1String("http://www.flickr.com")));
        QNetworkProxy proxy = QNetworkProxyFactory::systemProxyForQuery(query).value(0);
        if (proxy.type() != QNetworkProxy::NoProxy)
            QNetworkProxy::setApplicationProxy(proxy);
    }

    QDeclarativeView view;
    view.setSource(QUrl("qrc:/qml/flickr.qml"));

#if defined(FLICKR_FULLSCREEN)
    view.window()->showFullScreen();
#else
    view.window()->show();
#endif

    QObject::connect(view.engine(), SIGNAL(quit()), &view, SLOT(close()));
    return app.exec();
}
//![0]
