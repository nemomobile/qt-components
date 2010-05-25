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

#ifndef QBUTTONMODEL_P_H
#define QBUTTONMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Components API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/qbuttongroup.h>
#include <QtCore/qbasictimer.h>

class QButtonModelPrivate
{
    Q_DECLARE_PUBLIC(QButtonModel)
public:
    QButtonModelPrivate(QButtonModel *qq);
    void init();

    void emitPressed();
    void emitReleased();
    void emitClicked();

    void click();

#if 0 //ndef QT_NO_SHORTCUT
    QKeySequence shortcut;
    int shortcutId;
#endif

    uint checkable :1;
    uint checked :1;
    uint autoRepeat :1;
    uint autoExclusive :1;
    uint down :1;
    uint pressed :1;
    uint highlighted :1;

#ifndef QT_NO_BUTTONGROUP
    QButtonGroup* group;
#endif
    QBasicTimer repeatTimer;

    int autoRepeatDelay, autoRepeatInterval;

    QButtonModel *queryCheckedButton() const;
    QList<QButtonModel*>queryButtonList() const;
    void notifyChecked() const;

    QButtonModel *q_ptr;
};

#ifndef QT_NO_BUTTONGROUP

class QButtonGroupPrivate
{
    Q_DECLARE_PUBLIC(QButtonGroup)
public:
    QButtonGroupPrivate():exclusive(true){}
    QList<QAbstractButton *> buttonList;
    QPointer<QAbstractButton> checkedButton;
    void detectCheckedButton();
    void notifyChecked(QAbstractButton *button);
    bool exclusive;
    QMap<QAbstractButton*, int> mapping;

    QButtonGroup *q_ptr;
};

#endif

#endif // QBUTTONMODEL_P_H

