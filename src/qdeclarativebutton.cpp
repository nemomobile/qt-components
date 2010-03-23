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

#include "qdeclarativebutton.h"
#include "qdeclarativebutton_p.h"
#include "qbuttonmodel.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>

QDeclarativeButtonPrivate::QDeclarativeButtonPrivate() :
    QDeclarativeItemPrivate(),
    model(0)
{

}

void QDeclarativeButtonPrivate::createModel()
{
    Q_Q(QDeclarativeButton);

    model = new QButtonModel(q);
    q->connect(model, SIGNAL(released()), SIGNAL(released()));
    q->connect(model, SIGNAL(pressed()), SIGNAL(pressed()));
    q->connect(model, SIGNAL(clicked(bool)), SIGNAL(clicked(bool)));
    q->connect(model, SIGNAL(toggled(bool)), SIGNAL(toggled(bool)));
}

QDeclarativeButton::QDeclarativeButton(QDeclarativeItem *parent) :
    QDeclarativeItem(*new QDeclarativeButtonPrivate, parent)
{
    Q_D(QDeclarativeButton);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QDeclarativeButton::QDeclarativeButton(QDeclarativeButtonPrivate &dd, QDeclarativeItem *parent) :
    QDeclarativeItem(dd, parent)
{
    Q_D(QDeclarativeButton);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QDeclarativeButton::~QDeclarativeButton()
{
}

void QDeclarativeButton::setText(const QString &text)
{
    Q_D(QDeclarativeButton);
    if (d->text == text)
        return;

    d->text = text;
    emit textChanged(d->text);
}

QString QDeclarativeButton::text() const
{
    Q_D(const QDeclarativeButton);
    return d->text;
}

void QDeclarativeButton::setCheckable(bool c)
{
    Q_D(QDeclarativeButton);
    d->model->setCheckable(c);
}

bool QDeclarativeButton::isCheckable() const
{
    Q_D(const QDeclarativeButton);
    return d->model->isCheckable();
}

void QDeclarativeButton::setChecked(bool c)
{
    Q_D(QDeclarativeButton);
    d->model->setChecked(c);
}

bool QDeclarativeButton::isChecked() const
{
    Q_D(const QDeclarativeButton);
    return d->model->isChecked();
}

void QDeclarativeButton::setAutoRepeat(bool autoRepeat)
{
    Q_D(QDeclarativeButton);
    d->model->setAutoRepeat(autoRepeat);
}

bool QDeclarativeButton::autoRepeat() const
{
    Q_D(const QDeclarativeButton);
    return d->model->autoRepeat();
}

void QDeclarativeButton::setAutoExclusive(bool autoExclusive)
{
    Q_D(QDeclarativeButton);
    d->model->setAutoExclusive(autoExclusive);
}

bool QDeclarativeButton::autoExclusive() const
{
    Q_D(const QDeclarativeButton);
    return d->model->autoExclusive();
}

void QDeclarativeButton::setAutoRepeatDelay(int delay)
{
    Q_D(QDeclarativeButton);
    d->model->setAutoRepeatDelay(delay);
}

int QDeclarativeButton::autoRepeatDelay() const
{
    Q_D(const QDeclarativeButton);
    return d->model->autoRepeatDelay();
}

void QDeclarativeButton::setAutoRepeatInterval(int interval)
{
    Q_D(QDeclarativeButton);
    d->model->setAutoRepeatInterval(interval);
}

int QDeclarativeButton::autoRepeatInterval() const
{
    Q_D(const QDeclarativeButton);
    return d->model->autoRepeatInterval();
}

void QDeclarativeButtonPopulator::populate(QGraphicsObject *component, QObject *model)
{
    QDeclarativeButton *declarativebutton = static_cast<QDeclarativeButton *>(component);
    QPushButton *button = new QPushButton(declarativebutton->text());
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(declarativebutton);
    proxy->setWidget(button);
    model->connect(button, SIGNAL(clicked()), SIGNAL(clicked()));

    // ### Create event grabber primitive instead of QPushButton
    // ### Create data binding between component->text() and the right primitive
}

STYLE_REGISTER_COMPONENT_POPULATOR(QDeclarativeButton, QDeclarativeButtonPopulator);
