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

#ifndef MSTYLEWRAPPER_H
#define MSTYLEWRAPPER_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QVariant>

#include <mwidgetstyle.h>


class MStyleWrapper : public QObject
{
    Q_OBJECT

    // To be set from QML
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString styleClass READ styleClass WRITE setStyleClass NOTIFY styleClassChanged)
    Q_PROPERTY(QString styleType READ styleType WRITE setStyleType NOTIFY styleTypeChanged)
    Q_PROPERTY(QString styleObjectName READ styleObjectName WRITE setStyleObjectName NOTIFY styleObjectNameChanged)
    Q_PROPERTY(QString styleParentClass READ styleParentClass WRITE setStyleParentClass NOTIFY styleParentClassChanged)

    // We want to export the currentStyle as a plain QObject. QML can use
    // its properties to get the style data.
    Q_PROPERTY(QObject *current READ currentStyleAsObject NOTIFY currentStyleChanged)

    // Some pre-processed data that we want to export to QML as well. This could be
    // calculated inside QML, its convenient to have it done in one place.
    Q_PROPERTY(int preferredWidth READ preferredWidth NOTIFY currentStyleChanged)
    Q_PROPERTY(int preferredHeight READ preferredHeight NOTIFY currentStyleChanged)

public:
    MStyleWrapper(QObject *parent = 0);
    virtual ~MStyleWrapper();

    QString mode() const;
    void setMode(const QString &mode);

    QString styleClass() const;
    void setStyleClass(const QString &styleClass);

    QString styleType() const;
    void setStyleType(const QString &styleType);

    QString styleObjectName() const;
    void setStyleObjectName(const QString &styleObjectName);

    QString styleParentClass() const;
    void setStyleParentClass(const QString &styleParentClass);

    const MStyle *currentStyle() const;
    QObject *currentStyleAsObject();

    int preferredWidth() const;
    int preferredHeight() const;

    Q_INVOKABLE void feedback(const QByteArray& feedbackFunctionName);

Q_SIGNALS:
    void modeChanged();
    void styleClassChanged();
    void styleTypeChanged();
    void styleObjectNameChanged();
    void styleParentClassChanged();

    void currentStyleChanged();

public Q_SLOTS:
    QVariant get(const QString &propertyName);

private:
    friend class MThemeBridge;

    void updateStyle();
    void invalidateStyle();

    QString m_mode;
    QString m_styleClass;
    QString m_styleType;
    QString m_styleObjectName;
    QString m_styleParentClass;

    const MStyle *m_currentStyle[2];
    QHash<QString, const MStyle *> m_cachedStyles[2];
};

#endif // MSTYLEWRAPPER_H
