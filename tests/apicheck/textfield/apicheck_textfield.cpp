/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
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

void ApiCheckTextField::errorHighlight()
{
    validateProperty("errorHighlight", QVariant::Bool, false);
}

void ApiCheckTextField::maximumLength()
{
    validateProperty("maximumLength", QVariant::Int);
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
