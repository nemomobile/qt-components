/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
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
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QTDECLARATIVETOPLEVELITEM_P_H
#define QTDECLARATIVETOPLEVELITEM_P_H

#include <QObject>
#include <QPointer>

class QDeclarativeItem;
class QtDeclarativeTopLevelItem;
class QtDeclarativeTopLevelItemPrivate : public QObject
{
    Q_OBJECT

public:
    QtDeclarativeTopLevelItemPrivate();
    virtual ~QtDeclarativeTopLevelItemPrivate();

    void clearDependencyList();
    void setZFromSiblings();

    QDeclarativeItem *targetItem;
    QList<QDeclarativeItem *> dependencyList;
    uint transformDirty : 1;
    uint keepInside: 1;

protected:
    QtDeclarativeTopLevelItem *q_ptr;

private Q_SLOTS:
    void initDependencyList();
    void scheduleUpdateTransform();
    void updateTransform();
    void updateOpacity();
    void updateVisible();
    void updateWidthFromTarget();
    void updateHeightFromTarget();

    void updateParent();
    // void itemDestroyed();

private:
    Q_DECLARE_PUBLIC(QtDeclarativeTopLevelItem);
};

#endif // QTDECLARATIVETOPLEVELITEM_P_H
