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

// This class centralize work that we need that is shared by many components on
// the binding. For example, when the theme changes, we want to perform some actions
// on our style wrappers, in a specific order.

#ifndef MTHEMEBRIDGE_H
#define MTHEMEBRIDGE_H

#include <QObject>

class MDeclarativeIcon;
class MStyleWrapper;

class MThemeBridge : public QObject {
    Q_OBJECT

public:
    virtual ~MThemeBridge();

    static MThemeBridge *instance();

    void registerStyleWrapper(MStyleWrapper *wrapper);
    void unregisterStyleWrapper(MStyleWrapper *wrapper);

    void registerIcon(MDeclarativeIcon *icon);
    void unregisterIcon(MDeclarativeIcon *icon);

public Q_SLOTS:
    void updateResources();
    void updateCurrentStyle();

private:
    // MThemeBridge::instance() should be used instead.
    MThemeBridge();

    static MThemeBridge *m_self;
    QList<MStyleWrapper *> m_registeredStyleWrappers;
    QList<MDeclarativeIcon *> m_registeredIcons;
};

#endif // MTHEMEBRIDGE_H
