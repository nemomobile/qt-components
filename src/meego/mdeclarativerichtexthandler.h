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

#ifndef MDECLARATIVERICHTEXTHANDLER_H
#define MDECLARATIVERICHTEXTHANDLER_H

#include <QObject>
#include <QFont>
#include <QTextCharFormat>

class QTextCursor;
class QTextDocument;

class MDeclarativeRichTextHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString richText READ richText WRITE setRichText NOTIFY richTextChanged)
    Q_PROPERTY(QFont defaultFont READ defaultFont WRITE setDefaultFont NOTIFY defaultFontChanged)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline NOTIFY underlineChanged)
    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd)

public:
    MDeclarativeRichTextHandler(QObject *parent=0);
    virtual ~MDeclarativeRichTextHandler();

    QString richText() const;
    void setRichText(const QString &richText);

    QFont defaultFont() const;
    void setDefaultFont(const QFont &font);

    bool underline() const;
    void setUnderline(bool fontUnderline);

    bool bold() const;
    void setBold(bool fontBold);

    bool italic() const;
    void setItalic(bool fontItalic);

    int selectionStart() const;
    void setSelectionStart(int selectionStart);
    int selectionEnd() const;
    void setSelectionEnd(int selectionEnd);

    Q_INVOKABLE void cleanup();

public slots:
    void setSelection(int selectionBegin, int selectionEnd);

signals:
    void richTextChanged();
    void underlineChanged();
    void boldChanged();
    void italicChanged();
    void defaultFontChanged();

private:
    bool isUnderline(const QTextCharFormat& format) const;
    bool isBold(const QTextCharFormat& format) const;
    bool isItalic(const QTextCharFormat& format) const;
    void update();

    QTextDocument *m_document;
    QTextCursor *m_cursor;
    bool m_bold;
    bool m_italic;
    bool m_underline;
};

#endif //MDECLARATIVERICHTEXTHANDLER_H
