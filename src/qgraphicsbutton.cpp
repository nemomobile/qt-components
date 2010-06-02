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


QGraphicsButtonPrivate::QGraphicsButtonPrivate(QGraphicsButton *qq) :
    q_ptr(qq), model(0)
{

}

QGraphicsButtonPrivate::~QGraphicsButtonPrivate()
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
    QGraphicsWidget(parent, 0), m_dptr(new QGraphicsButtonPrivate(this))
{
    Q_D(QGraphicsButton);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QGraphicsButton::QGraphicsButton(QGraphicsButtonPrivate &dd, QGraphicsWidget *parent) :
    QGraphicsWidget(parent, 0), m_dptr(&dd)
{
    Q_D(QGraphicsButton);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QGraphicsButton::~QGraphicsButton()
{
    delete m_dptr;
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


void QGraphicsButtonPopulator::populate(QGraphicsObject *component, QObject *model)
{
    Q_UNUSED(model);

    // In this example, we created a big button primitive that does all the painting
    // instead of creating it with smaller primitives...
    ButtonPrimitive *p = new ButtonPrimitive(0, Qt::yellow, component);
    QObject::connect(component, SIGNAL(textChanged(const QString &)),
                     p, SLOT(setText(const QString &)));

    // ###
    QObject::connect(p, SIGNAL(buttonDown(bool)),
                     model, SLOT(setMousePressed(bool)));
    QObject::connect(p, SIGNAL(buttonDown(bool)),
                     model, SLOT(setMouseOver(bool)));
}

STYLE_REGISTER_COMPONENT_POPULATOR(QGraphicsButton, QGraphicsButtonPopulator);

