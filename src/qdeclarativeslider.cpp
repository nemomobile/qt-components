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

#include "qdeclarativeslider.h"
#include "qdeclarativeslider_p.h"
#include "qrangemodel.h"
#include <QSlider>
#include <QGraphicsProxyWidget>

QDeclarativeSliderPrivate::QDeclarativeSliderPrivate() :
    QDeclarativeItemPrivate(),
    model(0)
{

}

void QDeclarativeSliderPrivate::createModel()
{
    Q_Q(QDeclarativeSlider);

    model = new QRangeModel(q);
    q->connect(model, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
    q->connect(model, SIGNAL(rangeChanged(int, int)), SIGNAL(rangeChanged(int, int)));
}

QDeclarativeSlider::QDeclarativeSlider(QDeclarativeItem *parent) :
    QDeclarativeItem(*new QDeclarativeSliderPrivate, parent)
{
    Q_D(QDeclarativeSlider);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QDeclarativeSlider::QDeclarativeSlider(QDeclarativeSliderPrivate &dd, QDeclarativeItem *parent) :
    QDeclarativeItem(dd, parent)
{
    Q_D(QDeclarativeSlider);
    d->createModel();

    ComponentStyle::instance()->populate(this, d->model);
}

QDeclarativeSlider::~QDeclarativeSlider()
{
}

void QDeclarativeSlider::setMinimum(int min)
{
    Q_D(QDeclarativeSlider);
    d->model->setMinimum(min);
}

int QDeclarativeSlider::minimum() const
{
    Q_D(const QDeclarativeSlider);
    return d->model->minimum();
}

void QDeclarativeSlider::setMaximum(int max)
{
    Q_D(QDeclarativeSlider);
    d->model->setMaximum(max);
}

int QDeclarativeSlider::maximum() const
{
    Q_D(const QDeclarativeSlider);
    return d->model->maximum();
}

void QDeclarativeSlider::setRange(int min, int max)
{
    Q_D(QDeclarativeSlider);
    d->model->setRange(min, max);
}

void QDeclarativeSlider::setSingleStep(int step)
{
    Q_D(QDeclarativeSlider);
    d->model->setSingleStep(step);
}

int QDeclarativeSlider::singleStep() const
{
    Q_D(const QDeclarativeSlider);
    return d->model->singleStep();
}

void QDeclarativeSlider::setPageStep(int interval)
{
    Q_D(QDeclarativeSlider);
    d->model->setPageStep(interval);
}

int QDeclarativeSlider::pageStep() const
{
    Q_D(const QDeclarativeSlider);
    return d->model->pageStep();
}

void QDeclarativeSlider::setValue(int value)
{
    Q_D(QDeclarativeSlider);
    d->model->setValue(value);
}

int QDeclarativeSlider::value() const
{
    Q_D(const QDeclarativeSlider);
    return d->model->value();
}

void QDeclarativeSlider::setTracking(bool autoRepeat)
{
    Q_D(QDeclarativeSlider);
    d->model->setTracking(autoRepeat);
}

bool QDeclarativeSlider::hasTracking() const
{
    Q_D(const QDeclarativeSlider);
    return d->model->isTracking();
}

void QDeclarativeSliderPopulator::populate(QGraphicsObject *component, QObject *model)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(component);
    proxy->setWidget(slider);

    // ### Create event grabber primitive instead of QSlider
    model->connect(slider, SIGNAL(sliderMoved(int)), SLOT(setValue(int)));
    model->connect(slider, SIGNAL(valueChanged(int)), SLOT(setValue(int)));

    // ### How could I set the default size for a populated component?
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component);
    if (!item)
        return;

    item->setWidth(300);
    item->setHeight(50);
}

STYLE_REGISTER_COMPONENT_POPULATOR(QDeclarativeSlider, QDeclarativeSliderPopulator);
