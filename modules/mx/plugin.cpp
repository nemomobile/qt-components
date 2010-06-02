/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtDeclarative/qdeclarativeextensionplugin.h>
#include <QtDeclarative/qdeclarative.h>

#include "mxlikewindow.h"
#include "mxlikebuttongroup.h"
#include "mxtoplevelitem.h"

class QmlMxLikeModule : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    virtual void registerTypes(const char *uri)
    {
        qmlRegisterType<QmlMxLikeScreen>(uri,1,0,"WindowModel");
        qmlRegisterType<MxTopLevelItem>(uri,1,0,"TopLevelItemHelper");

        // ### Temporary solution until we can access children items inside QML
        qmlRegisterType<MxLikeButtonGroup>(uri, 1,0, "ButtonGroup");
    }
};

#include "plugin.moc"
Q_EXPORT_PLUGIN2(mxlikeplugin, QmlMxLikeModule);
