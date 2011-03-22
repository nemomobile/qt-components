/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SETTINGSWINDOW_P_H
#define SETTINGSWINDOW_P_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QLineEdit)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QDeclarativeView)

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QDeclarativeView *view);
    ~SettingsWindow();

private slots:
    void changeBordersVisibility(int index);
    void changeResolution(int index);
    void displayChanged();
    void userChangedDisplayValues();
    void userEditedDisplayValues();
    void userEditingFinished();

private:
    int activeDisplayProfile();
    QComboBox *resolutionComboBox;
    QComboBox *orientationComboBox;
    QComboBox *sensorComboBox;
    QComboBox *bordersComboBox;

    QLineEdit *dpiLineEdit;
    QLineEdit *inchLineEdit;
    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QCheckBox *orientationLockedCheckBox;
    bool updatingValuesBasedOnDisplayChange;
    bool userValueChanged;
    QObject *screen;

    QDeclarativeView *declarativeView;
};

#endif // SETTINGSWINDOW_P_H
