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

#ifndef APICHECK_TEXTFIELD_H
#define APICHECK_TEXTFIELD_H

#include "apicheckbase.h"

class ApiCheckTextField : public ApiCheckBase
{
    Q_OBJECT

public:
    ApiCheckTextField(QDeclarativeEngine *engine, const QString &module)
        : ApiCheckBase(engine, module) { }

private slots:
    void initTestCase();
    void placeholderText();
    void inputMethodHints();
    void font();
    void cursorPosition();
    void readOnly();
    void echoMode();
    void acceptableInput();
    void inputMask();
    void validator();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void text();
    void copy();
    void paste();
    void cut();
    void select();
    void selectAll();
    void selectWord();
    void positionAt();
    void positionToRectangle();
};

#endif
