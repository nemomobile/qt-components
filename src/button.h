/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef BUTTON_H
#define BUTTON_H

#include "teststyle.h"

#include <QtDeclarative/QDeclarativeItem>
#include <QtGui/QColor>

class Button : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Button(QColor color = Qt::red, QDeclarativeItem *parent = 0);

    QColor color() const { return m_color; }
    void setColor(QColor color) {
        if (m_color != color) {
            m_color = color;
            emit colorChanged();
        }
    }

Q_SIGNALS:
    void colorChanged();

private:
    QColor m_color;
};


class ButtonTestPopulator : public TestComponentPopulator
{
public:
    virtual void populate(QGraphicsObject *component, QObject *model);
};

#endif // BUTTON_H
