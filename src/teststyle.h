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

#ifndef TESTSTYLE_H
#define TESTSTYLE_H

#include "style.h"

#include <QtGui/QColor>

class QGraphicsItem;
class QGraphicsWidget;

class TestStyle : public ComponentStyle
{
    Q_OBJECT

public:
    TestStyle(QObject *parent = 0);
};

class TestComponentPopulator : public ComponentPopulator
{
public:
    static const char *styleName;

protected:
    QGraphicsWidget *createRectangle(int border, const QColor &color, QGraphicsItem *parent);
};

#endif // TESTSTYLE_H
