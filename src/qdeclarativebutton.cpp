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

// ### Just an experiment of connecting properties in C++
class PropertyBinder : public QObject
{
    Q_OBJECT

public:
    // We store the 'to' value as the parent() and the 'from' will be
    // the sender.

    PropertyBinder(QObject *from, const char *fromProperty,
                   QObject *to, const char *toProperty) {
        bind(from, fromProperty, NULL, to, toProperty);
    }

    PropertyBinder(QObject *from, const char *fromProperty, const char *fromSignal,
                   QObject *to, const char *toProperty) {
        bind(from, fromProperty, fromSignal, to, toProperty);
    }

    void bind(QObject *from, const char *fromProperty, const char *fromSignal,
              QObject *to, const char *toProperty) {
        // If 'to' (parent) or 'from' are destroyed, the binder will be as well.
        setParent(to);
        QObject::connect(from, SIGNAL(destroyed()), this, SLOT(deleteLater()));

        const int fromIndex = from->metaObject()->indexOfProperty(fromProperty);
        m_fromProp = from->metaObject()->property(fromIndex);

        const int toIndex = to->metaObject()->indexOfProperty(toProperty);
        m_toProp = to->metaObject()->property(toIndex);

        int notifySignal;
        if (fromSignal) {
            notifySignal = from->metaObject()->indexOfSignal(fromSignal);
        } else if (m_fromProp.hasNotifySignal()) {
            notifySignal = m_fromProp.notifySignalIndex();
        } else {
            qWarning("PropertyBinder: '%s' of '%s' has no NOTIFY signal.",
                     m_fromProp.name(), from->metaObject()->className());
            return;
        }

        QMetaObject::connect(from, notifySignal,
                             this, metaObject()->indexOfSlot("updateValue()"));

        m_toProp.write(to, m_fromProp.read(from));
    }

private Q_SLOTS:
    // ### :-P
    void updateValue() {
        QObject *to = parent();
        QObject *from = QObject::sender();
        m_toProp.write(to, m_fromProp.read(from));

        //qWarning() << "In " << int(to) <<  " Changing " << m_toProp.name() << " to " << m_fromProp.read(from);
    }

private:
    QMetaProperty m_fromProp;
    QMetaProperty m_toProp;
};

void QDeclarativeButtonPopulator::populate(QGraphicsObject *component, QObject *model)
{
    QDeclarativeButton *declarativebutton = static_cast<QDeclarativeButton *>(component);
    QPushButton *button = new QPushButton(declarativebutton->text());
    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(declarativebutton);
    proxy->setWidget(button);

    // NOTE: this uses the proxied QPushButton as a "primitive", and
    // the PropertyBinders connect the properties providing a similar
    // way to construct components as Qml.
    //
    // One could simply build a custom class that connected to the
    // proper NOTIFY signals for the properties in the model/component
    // pair.

    new PropertyBinder(component, "text", button, "text");

    // ### down doesn't have a NOTIFY :-(, so we work around by propagating
    // its value when related signals are emitted.
    new PropertyBinder(button, "down", "pressed", model, "mousePressed");
    new PropertyBinder(button, "down", "released", model, "mousePressed");
    new PropertyBinder(button, "down", "pressed", model, "mouseOver");
    new PropertyBinder(button, "down", "released", model, "mouseOver");

    new PropertyBinder(model, "checkable", button, "checkable");
    new PropertyBinder(model, "checked", button, "checked");

    // ### How could I set the default size for a populated component?
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component);
    if (!item)
        return;

    item->setWidth(200);
    item->setHeight(60);

    // ### QProxyWidget bug ??
    button->resize(200, 60);


    // ### Create event grabber primitive instead of QPushButton
    // ### Create data binding between component->text() and the right primitive
}

// ###
#include "qdeclarativebutton.moc"

STYLE_REGISTER_COMPONENT_POPULATOR(QDeclarativeButton, QDeclarativeButtonPopulator);
