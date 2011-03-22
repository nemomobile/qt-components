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
#include "apicheck_textarea.h"


void ApiCheckTextArea::initTestCase()
{
    init("TextArea");
}

void ApiCheckTextArea::font()
{
    validateProperty("font", QVariant::Font);
}

void ApiCheckTextArea::cursorPosition()
{
    validateProperty("cursorPosition", QVariant::Int);
}

void ApiCheckTextArea::horizontalAlignment()
{
    validateProperty("horizontalAlignment", QVariant::Int);
}

void ApiCheckTextArea::verticalAlignment()
{
    validateProperty("verticalAlignment", QVariant::Int);
}

void ApiCheckTextArea::readOnly()
{
    validateProperty("readOnly", QVariant::Bool, false);
}

void ApiCheckTextArea::selectedText()
{
    validateProperty("selectedText", QVariant::String);
}

void ApiCheckTextArea::selectionEnd()
{
    validateProperty("selectionEnd", QVariant::Int);
}

void ApiCheckTextArea::selectionStart()
{
    validateProperty("selectionStart", QVariant::Int);
}

void ApiCheckTextArea::text()
{
    validateProperty("text", QVariant::String);
}

void ApiCheckTextArea::textFormat()
{
    validateProperty("textFormat", QVariant::Int);
}

void ApiCheckTextArea::wrapMode()
{
    validateProperty("wrapMode", QVariant::Int);
}

void ApiCheckTextArea::copy()
{
    validateMethod("copy()");
}

void ApiCheckTextArea::paste()
{
    validateMethod("paste()");
}

void ApiCheckTextArea::cut()
{
    validateMethod("cut()");
}

void ApiCheckTextArea::select()
{
    validateMethod("select(QVariant,QVariant)");
}

void ApiCheckTextArea::selectAll()
{
    validateMethod("selectAll()");
}

void ApiCheckTextArea::selectWord()
{
    validateMethod("selectWord()");
}

void ApiCheckTextArea::positionAt()
{
    validateMethod("positionAt(QVariant,QVariant)");
}

void ApiCheckTextArea::positionToRectangle()
{
    validateMethod("positionToRectangle(QVariant)");
}
