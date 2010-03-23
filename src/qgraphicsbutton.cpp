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

#include "qgraphicsbutton.h"
#include "qgraphicsbutton_p.h"
#include "qbuttonmodel.h"
#include "style.h"

#include <QtGui/QPushButton>
#include <QtGui/QGraphicsProxyWidget>


QGraphicsButtonPrivate::QGraphicsButtonPrivate() :
    QGraphicsWidgetPrivate(),
    model(0)
{

}

void QGraphicsButtonPrivate::createModel()
{
    Q_Q(QGraphicsButton);

    model = new QButtonModel(q);
    q->connect(model, SIGNAL(released()), SIGNAL(released()));
    q->connect(model, SIGNAL(pressed()), SIGNAL(pressed()));
    q->connect(model, SIGNAL(clicked(bool)), SIGNAL(clicked(bool)));
    q->connect(model, SIGNAL(toggled(bool)), SIGNAL(toggled(bool)));
}

QGraphicsButton::QGraphicsButton(QGraphicsWidget *parent) :
    QGraphicsWidget(*new QGraphicsButtonPrivate, parent, 0)
{
    Q_D(QGraphicsButton);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QGraphicsButton::QGraphicsButton(QGraphicsButtonPrivate &dd, QGraphicsWidget *parent) :
    QGraphicsWidget(dd, parent, 0)
{
    Q_D(QGraphicsButton);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QGraphicsButton::~QGraphicsButton()
{
}

void QGraphicsButton::setText(const QString &text)
{
    Q_D(QGraphicsButton);
    if (d->text == text)
        return;

    d->text = text;
    emit textChanged(d->text);
}

QString QGraphicsButton::text() const
{
    Q_D(const QGraphicsButton);
    return d->text;
}

void QGraphicsButton::setCheckable(bool c)
{
    Q_D(QGraphicsButton);
    d->model->setCheckable(c);
}

bool QGraphicsButton::isCheckable() const
{
    Q_D(const QGraphicsButton);
    return d->model->isCheckable();
}

void QGraphicsButton::setChecked(bool c)
{
    Q_D(QGraphicsButton);
    d->model->setChecked(c);
}

bool QGraphicsButton::isChecked() const
{
    Q_D(const QGraphicsButton);
    return d->model->isChecked();
}

void QGraphicsButton::setAutoRepeat(bool autoRepeat)
{
    Q_D(QGraphicsButton);
    d->model->setAutoRepeat(autoRepeat);
}

bool QGraphicsButton::autoRepeat() const
{
    Q_D(const QGraphicsButton);
    return d->model->autoRepeat();
}

void QGraphicsButton::setAutoExclusive(bool autoExclusive)
{
    Q_D(QGraphicsButton);
    d->model->setAutoExclusive(autoExclusive);
}

bool QGraphicsButton::autoExclusive() const
{
    Q_D(const QGraphicsButton);
    return d->model->autoExclusive();
}

void QGraphicsButton::setAutoRepeatDelay(int delay)
{
    Q_D(QGraphicsButton);
    d->model->setAutoRepeatDelay(delay);
}

int QGraphicsButton::autoRepeatDelay() const
{
    Q_D(const QGraphicsButton);
    return d->model->autoRepeatDelay();
}

void QGraphicsButton::setAutoRepeatInterval(int interval)
{
    Q_D(QGraphicsButton);
    d->model->setAutoRepeatInterval(interval);
}

int QGraphicsButton::autoRepeatInterval() const
{
    Q_D(const QGraphicsButton);
    return d->model->autoRepeatInterval();
}

class RectWidget : public QGraphicsWidget
{
public:
    RectWidget(int border, QColor color, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent), m_color(color), m_border(border) { }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->fillRect(rect().adjusted(m_border, m_border, -m_border, -m_border), m_color);
    }

private:
    QColor m_color;
    int m_border;
};


void QGraphicsButtonPopulator::populate(QGraphicsObject *component, QObject *model)
{
    Q_UNUSED(model);

    // ### ProxyWidget is not playing nice with the layouting or vice-versa. :-P
    // QPushButton *button = new QPushButton("Button");
    // QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(component);
    // proxy->setWidget(button);
    // model->connect(button, SIGNAL(clicked()), SIGNAL(clicked()));

    new RectWidget(0, Qt::red, component);

    // ### Create event grabber primitive instead of QPushButton
    // ### Access info from model (pressed) and/or component (text)
}

STYLE_REGISTER_COMPONENT_POPULATOR(QGraphicsButton, QGraphicsButtonPopulator);
