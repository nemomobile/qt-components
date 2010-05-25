/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QBUTTONMODEL_H
#define QBUTTONMODEL_H

#include <QtGui/qwidget.h>
#include <QtGui/qbuttongroup.h>
#include <QtGui/qgraphicsitem.h>

class QButtonModelPrivate;

class QButtonModel : public QObject
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
    Q_PROPERTY(bool pressed READ isPressed WRITE setPressed DESIGNABLE false NOTIFY pressedChange);

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

#ifndef QT_NO_BUTTONGROUP
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
