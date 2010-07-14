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

class MWidgetStyleContainer;

class MStyleWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mode READ mode WRITE setMode);
    Q_PROPERTY(QString styleType READ styleType WRITE setStyleType);

    Q_PROPERTY(QColor textColor READ textColor NOTIFY modeChanged);

    // Needs patch in libmeegotouch
    // Q_PROPERTY(QObject * internalStyle READ internalStyle);

public:
    MStyleWrapper(QObject *parent = 0);
    virtual ~MStyleWrapper();

    QString mode() const;
    void setMode(const QString &mode);

    QString styleType() const;
    void setStyleType(const QString &styleType);

    QColor textColor() const;

    const MWidgetStyleContainer *styleContainer() const;

    // Needs patch in libmeegotouch
    // QObject *internalStyle();

Q_SIGNALS:
    void modeChanged(const QString &newMode);

protected:
    void updateStyleMode();

    QString m_mode;
    QString m_styletype;
    MWidgetStyleContainer *m_stylecontainer;
};

#endif // MSTYLEWRAPPER_H
