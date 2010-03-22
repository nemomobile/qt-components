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

#ifndef QDECLARATIVEBUTTON_H
#define QDECLARATIVEBUTTON_H

#include <QtDeclarative/qdeclarativeitem.h>
#include "style.h"

class QDeclarativeButtonPrivate;

class QDeclarativeButton : public QDeclarativeItem
{
    Q_OBJECT;

    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked DESIGNABLE isCheckable NOTIFY toggled USER true)
    Q_PROPERTY(bool autoRepeat READ autoRepeat WRITE setAutoRepeat)
    Q_PROPERTY(bool autoExclusive READ autoExclusive WRITE setAutoExclusive)
    Q_PROPERTY(int autoRepeatDelay READ autoRepeatDelay WRITE setAutoRepeatDelay)
    Q_PROPERTY(int autoRepeatInterval READ autoRepeatInterval WRITE setAutoRepeatInterval)

public:
    QDeclarativeButton(QDeclarativeItem *parent = 0);
    virtual ~QDeclarativeButton();

    void setCheckable(bool);
    bool isCheckable() const;

    void setChecked(bool);
    bool isChecked() const;

    void setAutoRepeat(bool);
    bool autoRepeat() const;

    void setAutoExclusive(bool);
    bool autoExclusive() const;

    void setAutoRepeatDelay(int);
    int autoRepeatDelay() const;

    void setAutoRepeatInterval(int);
    int autoRepeatInterval() const;

Q_SIGNALS:
    void released();  // ### Do we need these? Is their level too low?
    void pressed();  // ###
    void clicked(bool checked = false);
    void toggled(bool checked = false);

protected:
    QDeclarativeButton(QDeclarativeButtonPrivate &dd, QDeclarativeItem *parent = 0);

private:
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeButton);
    Q_DISABLE_COPY(QDeclarativeButton);
};

class QDeclarativeButtonPopulator : public ComponentPopulator
{
public:
    virtual void populate(QGraphicsObject *component, QObject *model);
};

#endif
