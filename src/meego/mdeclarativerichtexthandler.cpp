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

#include "mdeclarativerichtexthandler.h"

#include <Qt>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>

MDeclarativeRichTextHandler::MDeclarativeRichTextHandler(QObject *parent)
    : QObject(parent),
      m_document(0),
      m_cursor(0)
{
    m_document = new QTextDocument();
    m_cursor = new QTextCursor(m_document);
    QObject::connect(m_document, SIGNAL(contentsChanged()), this, SIGNAL(richTextChanged()));
}

MDeclarativeRichTextHandler::~MDeclarativeRichTextHandler()
{
    delete m_cursor;
    delete m_document;
}

QString MDeclarativeRichTextHandler::richText() const
{
    return m_document->toHtml();
}

void MDeclarativeRichTextHandler::setRichText(const QString &newRichText)
{
    if (newRichText == richText())
        return;

    if (Qt::mightBeRichText(newRichText))
        m_document->setHtml(newRichText);
    else
        m_document->setHtml(Qt::convertFromPlainText(newRichText, Qt::WhiteSpacePre));

    update();
}

QFont MDeclarativeRichTextHandler::defaultFont() const
{
    return m_document->defaultFont();
}

void MDeclarativeRichTextHandler::setDefaultFont(const QFont &font)
{
    if (font != defaultFont()) {
        m_document->setDefaultFont(font);

        emit defaultFontChanged();
    }
}

bool MDeclarativeRichTextHandler::underline() const
{
    return m_underline;
}

void MDeclarativeRichTextHandler::setUnderline(bool fontUnderline)
{
    if (m_underline == fontUnderline)
        return;

    QTextCharFormat format = m_cursor->charFormat();
    if (fontUnderline)
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    else
        format.setUnderlineStyle(QTextCharFormat::NoUnderline);

    m_cursor->mergeCharFormat(format);
    m_underline = fontUnderline;

    emit underlineChanged();
}

bool MDeclarativeRichTextHandler::bold() const
{
    return m_bold;
}

void MDeclarativeRichTextHandler::setBold(bool fontBold)
{
    if (m_bold == fontBold)
        return;

    QTextCharFormat format = m_cursor->charFormat();

    if (fontBold)
        format.setFontWeight(QFont::Bold);
    else
        format.setFontWeight(QFont::Normal);

    m_cursor->mergeCharFormat(format);
    m_bold = fontBold;

    emit boldChanged();
}

bool MDeclarativeRichTextHandler::italic() const
{
    return m_italic;
}

void MDeclarativeRichTextHandler::setItalic(bool fontItalic)
{
    if (m_italic == fontItalic)
        return;

    QTextCharFormat format = m_cursor->charFormat();

    format.setFontItalic(fontItalic);
    m_cursor->mergeCharFormat(format);
    m_italic = fontItalic;

    emit italicChanged();
}

int MDeclarativeRichTextHandler::selectionStart() const
{
    return m_cursor->selectionStart();
}

void MDeclarativeRichTextHandler::setSelectionStart(int selectionStart)
{
    setSelection(selectionStart, selectionEnd());
}

int MDeclarativeRichTextHandler::selectionEnd() const
{
    return m_cursor->selectionEnd();
}

void MDeclarativeRichTextHandler::setSelectionEnd(int selectionEnd)
{
    setSelection(selectionStart(), selectionEnd);
}

void MDeclarativeRichTextHandler::setSelection(int selectionStart, int selectionEnd)
{
    if (selectionStart < 0 || selectionEnd < 0)
        return;

    if (selectionStart > selectionEnd)
        qSwap(selectionStart, selectionEnd);

    m_cursor->setPosition(selectionStart);
    m_cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, selectionEnd - selectionStart);

    update();
}

void MDeclarativeRichTextHandler::cleanup()
{
    m_document->setHtml(QString());
    update();
}

bool MDeclarativeRichTextHandler::isUnderline(const QTextCharFormat& format) const
{
    return format.fontUnderline();
}

bool MDeclarativeRichTextHandler::isBold(const QTextCharFormat& format) const
{
    return format.fontWeight() != QFont::Normal;
}

bool MDeclarativeRichTextHandler::isItalic(const QTextCharFormat& format) const
{
    return format.fontItalic();
}

void MDeclarativeRichTextHandler::update()
{
    QTextCharFormat format = m_cursor->charFormat();
    bool underline = isUnderline(format);
    bool bold = isBold(format);
    bool italic = isItalic(format);

    if (m_underline != underline) {
        m_underline = isUnderline(format);
        emit underlineChanged();
    }
    if (m_bold != bold) {
        m_bold = bold;
        emit boldChanged();
    }
    if (m_italic != italic) {
        m_italic = italic;
        emit italicChanged();
    }
}
