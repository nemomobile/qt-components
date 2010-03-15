/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
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
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qobject_p.h"
#include "qbuttongroup.h"
#include "QtCore/qbasictimer.h"

QT_BEGIN_NAMESPACE

class QButtonModelPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QButtonModel)
public:
    QButtonModelPrivate(QAbstractButton *button, QSizePolicy::ControlType type = QSizePolicy::DefaultType);
    QButtonModelPrivate(QGraphicsObject *graphicsObject);
    void init();

    void emitPressed();
    void emitReleased();
    void emitClicked();

    void click();
    void refresh();

    QAbstractButton *button;
    QGraphicsObject *graphicsObject;

#ifndef QT_NO_SHORTCUT
    QKeySequence shortcut;
    int shortcutId;
#endif

    uint checkable :1;
    uint checked :1;
    uint autoRepeat :1;
    uint autoExclusive :1;
    uint down :1;
    uint blockRefresh :1;
    uint pressed : 1;

#ifndef QT_NO_BUTTONGROUP
    QButtonGroup* group;
#endif
    QBasicTimer repeatTimer;

    int autoRepeatDelay, autoRepeatInterval;

    QSizePolicy::ControlType controlType;
    mutable QSize sizeHint;

    QAbstractButton *queryCheckedButton() const;
    QList<QAbstractButton *>queryButtonList() const;
};

#ifndef QT_NO_BUTTONGROUP

class QButtonGroupPrivate: public QObjectPrivate
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
};

#endif

QT_END_NAMESPACE

#endif // QBUTTONMODEL_P_H

