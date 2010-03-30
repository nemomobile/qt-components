/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
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
#include <QtDeclarative>

//QDI so we're in the scene and associated with a window
class QmlMxLikeScreen : public QDeclarativeItem
{

    Q_OBJECT
    public:
        QmlMxLikeScreen(QDeclarativeItem * parent=0);

    public slots:
        void resetMove();
        void resetResize();
        void dragPointMove(int x, int y);
        void dragPointResize(int x, int y);

    private:
        bool moveValid, resizeValid;
        QPoint lastMovePoint;
        QPoint lastResizePoint;
};

QML_DECLARE_TYPE(QmlMxLikeScreen);
