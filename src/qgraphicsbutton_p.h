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

#ifndef QGRAPHICSBUTTON_P_H
#define QGRAPHICSBUTTON_P_H

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

#include <QtGui/QGraphicsWidget>
#include <QtGui/QPainter>

// ### QGraphicsWidgetPrivate is not exported
//#include <QtGui/private/qgraphicswidget_p.h>

class QButtonModel;
class QGraphicsButton;

class QGraphicsButtonPrivate
{
    // ### QGraphicsWidgetPrivate is not exported
public:
    QGraphicsButtonPrivate(QGraphicsButton *qq);
    virtual ~QGraphicsButtonPrivate();
protected:
    QGraphicsButton *q_ptr;

public:
    QGraphicsButtonPrivate();
    void createModel();

    QButtonModel *model;
    QString text;

private:
    Q_DECLARE_PUBLIC(QGraphicsButton)
};

class ButtonPrimitive : public QGraphicsWidget
{
    Q_OBJECT

public:
    ButtonPrimitive(int border, QColor color, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent), m_color(color), m_border(border), m_down(false) { }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QColor c = m_down ? m_color.darker() : m_color;
        painter->fillRect(rect().adjusted(m_border, m_border, -m_border, -m_border), c);
        painter->drawText(rect(), Qt::AlignCenter, m_text);
    }

public Q_SLOTS:
    void setText(const QString &text) {
        if (text != m_text) {
            m_text = text;
            update();
        }
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        m_down = true;
        emit buttonDown(true);
        update();
    }

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        m_down = false;
        emit buttonDown(false);
        update();
    }

Q_SIGNALS:
    // ### just a convenient signal for experimenting with model, compare with
    // the usage of PropertyBinder helper class in QDeclarativeButton.
    void buttonDown(bool);

private:
    QColor m_color;
    int m_border;
    QString m_text;

    bool m_down;
};

#endif
