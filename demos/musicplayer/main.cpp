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
#include <QtDeclarative>
#include "mediakeysobserver.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("MusicPlayer");

    QDeclarativeView view;
    QDeclarativeContext *context = view.rootContext();

    qmlRegisterType<MediaKeysObserver>("MediaKeysObserver", 1, 0, "MediaKeysObserver");

#ifdef DOCUMENT_GALLERY
    context->setContextProperty("documentGallery", true);
#else
    context->setContextProperty("documentGallery", false);
#endif

    view.setSource(QUrl("qrc:///qml/mainwindow.qml"));

#if defined(FULLSCREEN) || defined(Q_WS_SIMULATOR)
    view.window()->showFullScreen();
#else
    view.window()->show();
#endif

    QObject::connect(view.engine(), SIGNAL(quit()), &view, SLOT(close()));

    return app.exec();
}
//![0]
