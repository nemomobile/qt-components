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

#ifndef QBUTTONMODEL_H
#define QBUTTONMODEL_H

#include <QtGui/qwidget.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qgraphicsitem.h>

#include <kernel/common.h>

class QButtonModelPrivate;

class Q_COMPONENTS_EXPORT QButtonModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable NOTIFY checkableChanged)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked DESIGNABLE isCheckable NOTIFY toggled USER true)
    Q_PROPERTY(bool autoRepeat READ autoRepeat WRITE setAutoRepeat)
    Q_PROPERTY(bool autoExclusive READ autoExclusive WRITE setAutoExclusive)
    Q_PROPERTY(int autoRepeatDelay READ autoRepeatDelay WRITE setAutoRepeatDelay)
    Q_PROPERTY(int autoRepeatInterval READ autoRepeatInterval WRITE setAutoRepeatInterval)
    Q_PROPERTY(bool down READ isDown WRITE setDown DESIGNABLE false NOTIFY downChanged)
    Q_PROPERTY(bool highlighted READ isHighlighted WRITE setHighlighted DESIGNABLE false NOTIFY highlightChanged);
    Q_PROPERTY(bool pressed READ isPressed WRITE setPressed DESIGNABLE false NOTIFY pressedChanged);

public:
    QButtonModel(QObject *parent = 0);
    virtual ~QButtonModel();

#if 0 //ndef QT_NO_SHORTCUT
    void setShortcut(const QKeySequence &key);
    QKeySequence shortcut() const;
#endif

    void setCheckable(bool);
    bool isCheckable() const;

    void setChecked(bool);
    bool isChecked() const;

    void setDown(bool);
    bool isDown() const;

    void setAutoRepeat(bool);
    bool autoRepeat() const;

    void setAutoRepeatDelay(int);
    int autoRepeatDelay() const;

    void setAutoRepeatInterval(int);
    int autoRepeatInterval() const;

    void setAutoExclusive(bool);
    bool autoExclusive() const;

    bool isPressed() const;
    bool isHighlighted() const;

#ifndef QTCOMPONENTS_NO_BUTTONGROUP
    QButtonGroup *group() const;
#endif

public Q_SLOTS:
    // ### Those are convenient for C++ connections but may clutter the API exported to QML...
    void setPressed(bool);
    void setHighlighted(bool);

Q_SIGNALS:
    void released();
    void pressed();
    void clicked(bool checked = false);
    void toggled(bool checked = false);

    // Notify signals
    void downChanged();
    void checkableChanged();
    void highlightChanged();
    void pressedChanged();

protected:
    QButtonModel(QButtonModelPrivate &dd, QObject* parent = 0);
    QScopedPointer<QButtonModelPrivate> d_ptr;

    virtual void nextCheckState(); // ###

private:
    Q_DECLARE_PRIVATE(QButtonModel)
    Q_DISABLE_COPY(QButtonModel)
};

//QML_DECLARE_TYPE(QButtonModel)

#endif // QBUTTONMODEL_H
