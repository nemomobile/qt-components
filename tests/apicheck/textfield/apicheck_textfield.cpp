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

#include <QtTest/QtTest>
#include "apicheck_textfield.h"


void ApiCheckTextField::initTestCase()
{
    init("TextField");
}

void ApiCheckTextField::placeholderText()
{
    validateProperty("placeholderText", QVariant::String);
}

void ApiCheckTextField::inputMethodHints()
{
    validateProperty("inputMethodHints", QVariant::Int);
}

void ApiCheckTextField::font()
{
    validateProperty("font", QVariant::Font);
}

void ApiCheckTextField::cursorPosition()
{
    validateProperty("cursorPosition", QVariant::Int);
}

void ApiCheckTextField::readOnly()
{
    validateProperty("readOnly", QVariant::Bool);
}

void ApiCheckTextField::echoMode()
{
    validateProperty("echoMode", QVariant::Int);
}

void ApiCheckTextField::acceptableInput()
{
    validateProperty("acceptableInput", QVariant::Bool);
}

void ApiCheckTextField::inputMask()
{
    validateProperty("inputMask", QVariant::String);
}

void ApiCheckTextField::validator()
{
    validateProperty("validator", "QValidator*");
}

void ApiCheckTextField::selectedText()
{
    validateProperty("selectedText", QVariant::String);
}

void ApiCheckTextField::selectionEnd()
{
    validateProperty("selectionEnd", QVariant::Int);
}

void ApiCheckTextField::selectionStart()
{
    validateProperty("selectionStart", QVariant::Int);
}

void ApiCheckTextField::text()
{
    validateProperty("text", QVariant::String);
}

void ApiCheckTextField::copy()
{
    validateMethod("copy()");
}

void ApiCheckTextField::paste()
{
    validateMethod("paste()");
}

void ApiCheckTextField::cut()
{
    validateMethod("cut()");
}

void ApiCheckTextField::select()
{
    validateMethod("select(QVariant,QVariant)");
}

void ApiCheckTextField::selectAll()
{
    validateMethod("selectAll()");
}

void ApiCheckTextField::selectWord()
{
    validateMethod("selectWord()");
}

void ApiCheckTextField::positionAt()
{
    validateMethod("positionAt(QVariant)");
}

void ApiCheckTextField::positionToRectangle()
{
    validateMethod("positionToRectangle(QVariant)");
}
