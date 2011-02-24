/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#ifndef SSTYLEWRAPPER_H
#define SSTYLEWRAPPER_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>

class SStyleWrapperPrivate;

class SStyleWrapper : public QObject
{
    Q_OBJECT

    // To be set from QML
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString styleClass READ styleClass WRITE setStyleClass NOTIFY styleClassChanged)
    Q_PROPERTY(QString styleObjectName READ styleObjectName WRITE setStyleObjectName NOTIFY styleObjectNameChanged)

    // We want to export the currentStyle as a plain QObject. QML can use
    // its properties to get the style data.
    Q_PROPERTY(QObject *current READ currentStyleAsObject NOTIFY currentStyleChanged)

    // Some pre-processed data that we want to export to QML as well. This could be
    // calculated inside QML, its convenient to have it done in one place.
    Q_PROPERTY(qreal preferredWidth READ preferredWidth NOTIFY currentStyleChanged)
    Q_PROPERTY(qreal preferredHeight READ preferredHeight NOTIFY currentStyleChanged)

public:
    enum FontRole {
        Undefined = 0,
        Primary,
        Secondary,
        Title,
        PrimarySmall,
        Digital
    };


    SStyleWrapper(QObject *parent = 0);
    ~SStyleWrapper();

    QString mode() const;
    void setMode(const QString &mode);

    QString styleClass() const;
    void setStyleClass(const QString &styleClass);

    QString styleType() const;
    void setStyleType(const QString &styleType);

    QString styleObjectName() const;
    void setStyleObjectName(const QString &styleObjectName);

    QObject *currentStyleAsObject() const;

    qreal preferredWidth() const;
    qreal preferredHeight() const;

    Q_INVOKABLE void play(int effect);
    Q_INVOKABLE qreal textWidth(const QString &text, const QFont &font) const;
    Q_INVOKABLE qreal fontHeight(const QFont &font) const;
    Q_INVOKABLE QVariant get(const QString &propertyName);
    bool eventFilter(QObject *watched, QEvent *event);

Q_SIGNALS:
    void modeChanged();
    void styleClassChanged();
    void styleTypeChanged();
    void styleObjectNameChanged();
    void currentStyleChanged();

protected:
    QScopedPointer<SStyleWrapperPrivate> d_ptr;

private:
    Q_PRIVATE_SLOT(d_func(), void _q_desktopWorkareaChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_displayChanged())

    Q_DISABLE_COPY(SStyleWrapper)
    Q_DECLARE_PRIVATE(SStyleWrapper)
};

#endif // SSTYLEWRAPPER_H
