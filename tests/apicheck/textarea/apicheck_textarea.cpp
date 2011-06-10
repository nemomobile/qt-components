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

void ApiCheckTextArea::errorHighlight()
{
    validateProperty("errorHighlight", QVariant::Bool, false);
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
void ApiCheckTextArea::inputMethodHints()
{
    validateProperty("inputMethodHints", QVariant::Int);
}
