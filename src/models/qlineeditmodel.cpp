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

#include "qlineeditmodel.h"

#ifndef QT_NO_LINEEDIT

#include "qclipboard.h"
#include "qapplication.h"

QT_BEGIN_NAMESPACE

/*!
    \internal

    Updates the display text based of the current edit text
    If the text has changed will emit displayTextChanged()
*/
void QLineEditModel::updateDisplayText()
{
    QString orig = m_textLayout.text();
    QString str;
    if (m_echoMode == QLineEdit::NoEcho)
        str = QString::fromLatin1("");
    else
        str = m_text;

    if (m_echoMode == QLineEdit::Password || (m_echoMode == QLineEdit::PasswordEchoOnEdit
                && !m_passwordEchoEditing))
        str.fill(m_passwordCharacter);

    // replace certain non-printable characters with spaces (to avoid
    // drawing boxes when using fonts that don't have glyphs for such
    // characters)
    QChar* uc = str.data();
    for (int i = 0; i < (int)str.length(); ++i) {
        if ((uc[i] < 0x20 && uc[i] != 0x09)
            || uc[i] == QChar::LineSeparator
            || uc[i] == QChar::ParagraphSeparator
            || uc[i] == QChar::ObjectReplacementCharacter)
            uc[i] = QChar(0x0020);
    }

    m_textLayout.setText(str);

    QTextOption option;
    option.setTextDirection(m_layoutDirection);
    option.setFlags(QTextOption::IncludeTrailingSpaces);
    m_textLayout.setTextOption(option);

    m_textLayout.beginLayout();
    QTextLine l = m_textLayout.createLine();
    m_textLayout.endLayout();
    m_ascent = qRound(l.ascent());

    if (str != orig)
        emit displayTextChanged(str);
}

#ifndef QT_NO_CLIPBOARD
/*!
    \internal

    Copies the currently selected text into the clipboard using the given
    \a mode.
  
    \note If the echo mode is set to a mode other than Normal then copy
    will not work.  This is to prevent using copy as a method of bypassing
    password features of the line control.
*/
void QLineEditModel::copy(QClipboard::Mode mode) const
{
    QString t = selectedText();
    if (!t.isEmpty() && m_echoMode == QLineEdit::Normal) {
        disconnect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, 0);
        QApplication::clipboard()->setText(t, mode);
        connect(QApplication::clipboard(), SIGNAL(selectionChanged()),
                   this, SLOT(_q_clipboardChanged()));
    }
}

/*!
    \internal

    Inserts the text stored in the application clipboard into the line
    control.

    \sa insert()
*/
void QLineEditModel::paste()
{
    QString clip = QApplication::clipboard()->text(QClipboard::Clipboard);
    if (!clip.isEmpty() || hasSelectedText()) {
        separate(); //make it a separate undo/redo command
        insert(clip);
        separate();
    }
}

#endif // !QT_NO_CLIPBOARD

/*!
    \internal

    Handles the behavior for the backspace key or function.
    Removes the current selection if there is a selection, otherwise
    removes the character prior to the cursor position.

    \sa del()
*/
void QLineEditModel::backspace()
{
    int priorState = m_undoState;
    if (hasSelectedText()) {
        removeSelectedText();
    } else if (m_cursor) {
            --m_cursor;
            if (m_maskData)
                m_cursor = prevMaskBlank(m_cursor);
            QChar uc = m_text.at(m_cursor);
            if (m_cursor > 0 && uc.unicode() >= 0xdc00 && uc.unicode() < 0xe000) {
                // second half of a surrogate, check if we have the first half as well,
                // if yes delete both at once
                uc = m_text.at(m_cursor - 1);
                if (uc.unicode() >= 0xd800 && uc.unicode() < 0xdc00) {
                    internalDelete(true);
                    --m_cursor;
                }
            }
            internalDelete(true);
    }
    finishChange(priorState);
}

/*!
    \internal

    Handles the behavior for the delete key or function.
    Removes the current selection if there is a selection, otherwise
    removes the character after the cursor position.

    \sa del()
*/
void QLineEditModel::del()
{
    int priorState = m_undoState;
    if (hasSelectedText()) {
        removeSelectedText();
    } else {
        int n = m_textLayout.nextCursorPosition(m_cursor) - m_cursor;
        while (n--)
            internalDelete();
    }
    finishChange(priorState);
}

/*!
    \internal

    Inserts the given \a newText at the current cursor position.
    If there is any selected text it is removed prior to insertion of
    the new text.
*/
void QLineEditModel::insert(const QString &newText)
{
    int priorState = m_undoState;
    removeSelectedText();
    internalInsert(newText);
    finishChange(priorState);
}

/*!
    \internal

    Clears the line control text.
*/
void QLineEditModel::clear()
{
    int priorState = m_undoState;
    m_selstart = 0;
    m_selend = m_text.length();
    removeSelectedText();
    separate();
    finishChange(priorState, /*update*/false, /*edited*/false);
}

/*!
    \internal

    Sets \a length characters from the given \a start position as selected.
    The given \a start position must be within the current text for
    the line control.  If \a length characters cannot be selected, then
    the selection will extend to the end of the current text.
*/
void QLineEditModel::setSelection(int start, int length)
{
    if(start < 0 || start > (int)m_text.length()){
        qWarning("QLineEditModel::setSelection: Invalid start position");
        return;
    }

    if (length > 0) {
        if (start == m_selstart && start + length == m_selend)
            return;
        m_selstart = start;
        m_selend = qMin(start + length, (int)m_text.length());
        m_cursor = m_selend;
    } else {
        if (start == m_selend && start + length == m_selstart)
            return;
        m_selstart = qMax(start + length, 0);
        m_selend = start;
        m_cursor = m_selstart;
    }
    emit selectionChanged();
    emitCursorPositionChanged();
}

void QLineEditModel::_q_clipboardChanged()
{
}

void QLineEditModel::_q_deleteSelected()
{
    if (!hasSelectedText())
        return;

    int priorState = m_undoState;
    removeSelectedText();
    separate();
    finishChange(priorState);
}

/*!
    \internal

    Initializes the line control with a starting text value of \a txt.
*/
void QLineEditModel::init(const QString &txt)
{
    m_text = txt;
    updateDisplayText();
    m_cursor = m_text.length();
}

/*!
    \internal

    Sets the password echo editing to \a editing.  If password echo editing
    is true, then the text of the password is displayed even if the echo
    mode is set to QLineEdit::PasswordEchoOnEdit.  Password echoing editing
    does not affect other echo modes.
*/
void QLineEditModel::updatePasswordEchoEditing(bool editing)
{
    m_passwordEchoEditing = editing;
    updateDisplayText();
}

/*!
    \internal

    Returns the cursor position of the given \a x pixel value in relation
    to the displayed text.  The given \a betweenOrOn specified what kind
    of cursor position is requested.
*/
int QLineEditModel::xToPos(int x, QTextLine::CursorPosition betweenOrOn) const
{
    return m_textLayout.lineAt(0).xToCursor(x, betweenOrOn);
}

/*!
    \internal

    Fixes the current text so that it is valid given any set validators.

    Returns true if the text was changed.  Otherwise returns false.
*/
bool QLineEditModel::fixup() // this function assumes that validate currently returns != Acceptable
{
#ifndef QT_NO_VALIDATOR
    if (m_validator) {
        QString textCopy = m_text;
        int cursorCopy = m_cursor;
        m_validator->fixup(textCopy);
        if (m_validator->validate(textCopy, cursorCopy) == QValidator::Acceptable) {
            if (textCopy != m_text || cursorCopy != m_cursor)
                internalSetText(textCopy, cursorCopy);
            return true;
        }
    }
#endif
    return false;
}

/*!
    \internal

    Moves the cursor to the given position \a pos.   If \a mark is true will
    adjust the currently selected text.
*/
void QLineEditModel::moveCursor(int pos, bool mark)
{
    if (pos != m_cursor) {
        separate();
        if (m_maskData)
            pos = pos > m_cursor ? nextMaskBlank(pos) : prevMaskBlank(pos);
    }
    if (mark) {
        int anchor;
        if (m_selend > m_selstart && m_cursor == m_selstart)
            anchor = m_selend;
        else if (m_selend > m_selstart && m_cursor == m_selend)
            anchor = m_selstart;
        else
            anchor = m_cursor;
        m_selstart = qMin(anchor, pos);
        m_selend = qMax(anchor, pos);
        updateDisplayText();
    } else {
        internalDeselect();
    }
    m_cursor = pos;
    if (mark || m_selDirty) {
        m_selDirty = false;
        emit selectionChanged();
    }
    emitCursorPositionChanged();
}

/*!
    \internal

    Sets the selection to cover the word at the given cursor position.
    The word boundries is defined by the behavior of QTextLayout::SkipWords
    cursor mode.
*/
void QLineEditModel::selectWordAtPos(int cursor)
{
    int c = m_textLayout.previousCursorPosition(cursor, QTextLayout::SkipWords);
    moveCursor(c, false);
    // ## text layout should support end of words.
    int end = m_textLayout.nextCursorPosition(cursor, QTextLayout::SkipWords);
    while (end > cursor && m_text[end-1].isSpace())
        --end;
    moveCursor(end, true);
}

/*!
    \internal

    Completes a change to the line control text.  If the change is not valid
    will undo the line control state back to the given \a validateFromState.

    If \a edited is true and the change is valid, will emit textEdited() in
    addition to textChanged().  Otherwise only emits textChanged() on a valid
    change.

    The \a update value is currently unused.
*/
bool QLineEditModel::finishChange(int validateFromState, bool update, bool edited)
{
    Q_UNUSED(update)
    bool lineDirty = m_selDirty;
    if (m_textDirty) {
        // do validation
        bool wasValidInput = m_validInput;
        m_validInput = true;
#ifndef QT_NO_VALIDATOR
        if (m_validator) {
            m_validInput = false;
            QString textCopy = m_text;
            int cursorCopy = m_cursor;
            m_validInput = (m_validator->validate(textCopy, cursorCopy) != QValidator::Invalid);
            if (m_validInput) {
                if (m_text != textCopy) {
                    internalSetText(textCopy, cursorCopy);
                    return true;
                }
                m_cursor = cursorCopy;
            }
        }
#endif
        if (validateFromState >= 0 && wasValidInput && !m_validInput) {
            if (m_transactions.count())
                return false;
            internalUndo(validateFromState);
            m_history.resize(m_undoState);
            if (m_modifiedState > m_undoState)
                m_modifiedState = -1;
            m_validInput = true;
            m_textDirty = false;
        }
        updateDisplayText();
        lineDirty |= m_textDirty;
        if (m_textDirty) {
            m_textDirty = false;
            QString actualText = text();
            if (edited)
                emit textEdited(actualText);
            emit textChanged(actualText);
        }
    }
    if (m_selDirty) {
        m_selDirty = false;
        emit selectionChanged();
    }
    emitCursorPositionChanged();
    return true;
}

/*!
    \internal

    An internal function for setting the text of the line control.
*/
void QLineEditModel::internalSetText(const QString &txt, int pos, bool edited)
{
    internalDeselect();
    QString oldText = m_text;
    if (m_maskData) {
        m_text = maskString(0, txt, true);
        m_text += clearString(m_text.length(), m_maxLength - m_text.length());
    } else {
        m_text = txt.isEmpty() ? txt : txt.left(m_maxLength);
    }
    m_history.clear();
    m_modifiedState =  m_undoState = 0;
    m_cursor = (pos < 0 || pos > m_text.length()) ? m_text.length() : pos;
    m_textDirty = (oldText != m_text);
    finishChange(-1, true, edited);
}


/*!
    \internal

    Adds the given \a command to the undo history
    of the line control.  Does not apply the command.
*/
void QLineEditModel::addCommand(const Command &cmd)
{
    if (m_separator && m_undoState && m_history[m_undoState - 1].type != Separator) {
        m_history.resize(m_undoState + 2);
        m_history[m_undoState++] = Command(Separator, m_cursor, 0, m_selstart, m_selend);
    } else {
        m_history.resize(m_undoState + 1);
    }
    m_separator = false;
    m_history[m_undoState++] = cmd;
}

/*!
    \internal

    Inserts the given string \a s into the line
    control.

    Also adds the appropriate commands into the undo history.
    This function does not call finishChange(), and may leave the text
    in an invalid state.
*/
void QLineEditModel::internalInsert(const QString &s)
{
    if (hasSelectedText())
        addCommand(Command(SetSelection, m_cursor, 0, m_selstart, m_selend));
    if (m_maskData) {
        QString ms = maskString(m_cursor, s);
        for (int i = 0; i < (int) ms.length(); ++i) {
            addCommand (Command(DeleteSelection, m_cursor + i, m_text.at(m_cursor + i), -1, -1));
            addCommand(Command(Insert, m_cursor + i, ms.at(i), -1, -1));
        }
        m_text.replace(m_cursor, ms.length(), ms);
        m_cursor += ms.length();
        m_cursor = nextMaskBlank(m_cursor);
        m_textDirty = true;
    } else {
        int remaining = m_maxLength - m_text.length();
        if (remaining != 0) {
            m_text.insert(m_cursor, s.left(remaining));
            for (int i = 0; i < (int) s.left(remaining).length(); ++i)
               addCommand(Command(Insert, m_cursor++, s.at(i), -1, -1));
            m_textDirty = true;
        }
    }
}

/*!
    \internal

    deletes a single character from the current text.  If \a wasBackspace,
    the character prior to the cursor is removed.  Otherwise the character
    after the cursor is removed.

    Also adds the appropriate commands into the undo history.
    This function does not call finishChange(), and may leave the text
    in an invalid state.
*/
void QLineEditModel::internalDelete(bool wasBackspace)
{
    if (m_cursor < (int) m_text.length()) {
        if (hasSelectedText())
            addCommand(Command(SetSelection, m_cursor, 0, m_selstart, m_selend));
        addCommand(Command((CommandType)((m_maskData ? 2 : 0) + (wasBackspace ? Remove : Delete)),
                   m_cursor, m_text.at(m_cursor), -1, -1));
        if (m_maskData) {
            m_text.replace(m_cursor, 1, clearString(m_cursor, 1));
            addCommand(Command(Insert, m_cursor, m_text.at(m_cursor), -1, -1));
        } else {
            m_text.remove(m_cursor, 1);
        }
        m_textDirty = true;
    }
}

/*!
    \internal

    removes the currently selected text from the line control.

    Also adds the appropriate commands into the undo history.
    This function does not call finishChange(), and may leave the text
    in an invalid state.
*/
void QLineEditModel::removeSelectedText()
{
    if (m_selstart < m_selend && m_selend <= (int) m_text.length()) {
        separate();
        int i ;
        addCommand(Command(SetSelection, m_cursor, 0, m_selstart, m_selend));
        if (m_selstart <= m_cursor && m_cursor < m_selend) {
            // cursor is within the selection. Split up the commands
            // to be able to restore the correct cursor position
            for (i = m_cursor; i >= m_selstart; --i)
                addCommand (Command(DeleteSelection, i, m_text.at(i), -1, 1));
            for (i = m_selend - 1; i > m_cursor; --i)
                addCommand (Command(DeleteSelection, i - m_cursor + m_selstart - 1, m_text.at(i), -1, -1));
        } else {
            for (i = m_selend-1; i >= m_selstart; --i)
                addCommand (Command(RemoveSelection, i, m_text.at(i), -1, -1));
        }
        if (m_maskData) {
            m_text.replace(m_selstart, m_selend - m_selstart,  clearString(m_selstart, m_selend - m_selstart));
            for (int i = 0; i < m_selend - m_selstart; ++i)
                addCommand(Command(Insert, m_selstart + i, m_text.at(m_selstart + i), -1, -1));
        } else {
            m_text.remove(m_selstart, m_selend - m_selstart);
        }
        if (m_cursor > m_selstart)
            m_cursor -= qMin(m_cursor, m_selend) - m_selstart;
        internalDeselect();
        m_textDirty = true;
    }
}

/*!
    \internal

    Parses the input mask specified by \a maskFields to generate
    the mask data used to handle input masks.
*/
void QLineEditModel::parseInputMask(const QString &maskFields)
{
    int delimiter = maskFields.indexOf(QLatin1Char(';'));
    if (maskFields.isEmpty() || delimiter == 0) {
        if (m_maskData) {
            delete [] m_maskData;
            m_maskData = 0;
            m_maxLength = 32767;
            internalSetText(QString());
        }
        return;
    }

    if (delimiter == -1) {
        m_blank = QLatin1Char(' ');
        m_inputMask = maskFields;
    } else {
        m_inputMask = maskFields.left(delimiter);
        m_blank = (delimiter + 1 < maskFields.length()) ? maskFields[delimiter + 1] : QLatin1Char(' ');
    }

    // calculate m_maxLength / m_maskData length
    m_maxLength = 0;
    QChar c = 0;
    for (int i=0; i<m_inputMask.length(); i++) {
        c = m_inputMask.at(i);
        if (i > 0 && m_inputMask.at(i-1) == QLatin1Char('\\')) {
            m_maxLength++;
            continue;
        }
        if (c != QLatin1Char('\\') && c != QLatin1Char('!') &&
             c != QLatin1Char('<') && c != QLatin1Char('>') &&
             c != QLatin1Char('{') && c != QLatin1Char('}') &&
             c != QLatin1Char('[') && c != QLatin1Char(']'))
            m_maxLength++;
    }

    delete [] m_maskData;
    m_maskData = new MaskInputData[m_maxLength];

    MaskInputData::Casemode m = MaskInputData::NoCaseMode;
    c = 0;
    bool s;
    bool escape = false;
    int index = 0;
    for (int i = 0; i < m_inputMask.length(); i++) {
        c = m_inputMask.at(i);
        if (escape) {
            s = true;
            m_maskData[index].maskChar = c;
            m_maskData[index].separator = s;
            m_maskData[index].caseMode = m;
            index++;
            escape = false;
        } else if (c == QLatin1Char('<')) {
                m = MaskInputData::Lower;
        } else if (c == QLatin1Char('>')) {
            m = MaskInputData::Upper;
        } else if (c == QLatin1Char('!')) {
            m = MaskInputData::NoCaseMode;
        } else if (c != QLatin1Char('{') && c != QLatin1Char('}') && c != QLatin1Char('[') && c != QLatin1Char(']')) {
            switch (c.unicode()) {
            case 'A':
            case 'a':
            case 'N':
            case 'n':
            case 'X':
            case 'x':
            case '9':
            case '0':
            case 'D':
            case 'd':
            case '#':
            case 'H':
            case 'h':
            case 'B':
            case 'b':
                s = false;
                break;
            case '\\':
                escape = true;
            default:
                s = true;
                break;
            }

            if (!escape) {
                m_maskData[index].maskChar = c;
                m_maskData[index].separator = s;
                m_maskData[index].caseMode = m;
                index++;
            }
        }
    }
    internalSetText(m_text);
}


/*!
    \internal

    checks if the key is valid compared to the inputMask
*/
bool QLineEditModel::isValidInput(QChar key, QChar mask) const
{
    switch (mask.unicode()) {
    case 'A':
        if (key.isLetter())
            return true;
        break;
    case 'a':
        if (key.isLetter() || key == m_blank)
            return true;
        break;
    case 'N':
        if (key.isLetterOrNumber())
            return true;
        break;
    case 'n':
        if (key.isLetterOrNumber() || key == m_blank)
            return true;
        break;
    case 'X':
        if (key.isPrint())
            return true;
        break;
    case 'x':
        if (key.isPrint() || key == m_blank)
            return true;
        break;
    case '9':
        if (key.isNumber())
            return true;
        break;
    case '0':
        if (key.isNumber() || key == m_blank)
            return true;
        break;
    case 'D':
        if (key.isNumber() && key.digitValue() > 0)
            return true;
        break;
    case 'd':
        if ((key.isNumber() && key.digitValue() > 0) || key == m_blank)
            return true;
        break;
    case '#':
        if (key.isNumber() || key == QLatin1Char('+') || key == QLatin1Char('-') || key == m_blank)
            return true;
        break;
    case 'B':
        if (key == QLatin1Char('0') || key == QLatin1Char('1'))
            return true;
        break;
    case 'b':
        if (key == QLatin1Char('0') || key == QLatin1Char('1') || key == m_blank)
            return true;
        break;
    case 'H':
        if (key.isNumber() || (key >= QLatin1Char('a') && key <= QLatin1Char('f')) || (key >= QLatin1Char('A') && key <= QLatin1Char('F')))
            return true;
        break;
    case 'h':
        if (key.isNumber() || (key >= QLatin1Char('a') && key <= QLatin1Char('f')) || (key >= QLatin1Char('A') && key <= QLatin1Char('F')) || key == m_blank)
            return true;
        break;
    default:
        break;
    }
    return false;
}

/*!
    \internal

    Returns true if the given text \a str is valid for any
    validator or input mask set for the line control.

    Otherwise returns false
*/
bool QLineEditModel::hasAcceptableInput(const QString &str) const
{
#ifndef QT_NO_VALIDATOR
    QString textCopy = str;
    int cursorCopy = m_cursor;
    if (m_validator && m_validator->validate(textCopy, cursorCopy)
        != QValidator::Acceptable)
        return false;
#endif

    if (!m_maskData)
        return true;

    if (str.length() != m_maxLength)
        return false;

    for (int i=0; i < m_maxLength; ++i) {
        if (m_maskData[i].separator) {
            if (str.at(i) != m_maskData[i].maskChar)
                return false;
        } else {
            if (!isValidInput(str.at(i), m_maskData[i].maskChar))
                return false;
        }
    }
    return true;
}

/*!
    \internal

    Applies the inputMask on \a str starting from position \a pos in the mask. \a clear
    specifies from where characters should be gotten when a separator is met in \a str - true means
    that blanks will be used, false that previous input is used.
    Calling this when no inputMask is set is undefined.
*/
QString QLineEditModel::maskString(uint pos, const QString &str, bool clear) const
{
    if (pos >= (uint)m_maxLength)
        return QString::fromLatin1("");

    QString fill;
    fill = clear ? clearString(0, m_maxLength) : m_text;

    int strIndex = 0;
    QString s = QString::fromLatin1("");
    int i = pos;
    while (i < m_maxLength) {
        if (strIndex < str.length()) {
            if (m_maskData[i].separator) {
                s += m_maskData[i].maskChar;
                if (str[(int)strIndex] == m_maskData[i].maskChar)
                    strIndex++;
                ++i;
            } else {
                if (isValidInput(str[(int)strIndex], m_maskData[i].maskChar)) {
                    switch (m_maskData[i].caseMode) {
                    case MaskInputData::Upper:
                        s += str[(int)strIndex].toUpper();
                        break;
                    case MaskInputData::Lower:
                        s += str[(int)strIndex].toLower();
                        break;
                    default:
                        s += str[(int)strIndex];
                    }
                    ++i;
                } else {
                    // search for separator first
                    int n = findInMask(i, true, true, str[(int)strIndex]);
                    if (n != -1) {
                        if (str.length() != 1 || i == 0 || (i > 0 && (!m_maskData[i-1].separator || m_maskData[i-1].maskChar != str[(int)strIndex]))) {
                            s += fill.mid(i, n-i+1);
                            i = n + 1; // update i to find + 1
                        }
                    } else {
                        // search for valid m_blank if not
                        n = findInMask(i, true, false, str[(int)strIndex]);
                        if (n != -1) {
                            s += fill.mid(i, n-i);
                            switch (m_maskData[n].caseMode) {
                            case MaskInputData::Upper:
                                s += str[(int)strIndex].toUpper();
                                break;
                            case MaskInputData::Lower:
                                s += str[(int)strIndex].toLower();
                                break;
                            default:
                                s += str[(int)strIndex];
                            }
                            i = n + 1; // updates i to find + 1
                        }
                    }
                }
                ++strIndex;
            }
        } else
            break;
    }

    return s;
}



/*!
    \internal

    Returns a "cleared" string with only separators and blank chars.
    Calling this when no inputMask is set is undefined.
*/
QString QLineEditModel::clearString(uint pos, uint len) const
{
    if (pos >= (uint)m_maxLength)
        return QString();

    QString s;
    int end = qMin((uint)m_maxLength, pos + len);
    for (int i = pos; i < end; ++i)
        if (m_maskData[i].separator)
            s += m_maskData[i].maskChar;
        else
            s += m_blank;

    return s;
}

/*!
    \internal

    Strips blank parts of the input in a QLineEditModel when an inputMask is set,
    separators are still included. Typically "127.0__.0__.1__" becomes "127.0.0.1".
*/
QString QLineEditModel::stripString(const QString &str) const
{
    if (!m_maskData)
        return str;

    QString s;
    int end = qMin(m_maxLength, (int)str.length());
    for (int i = 0; i < end; ++i)
        if (m_maskData[i].separator)
            s += m_maskData[i].maskChar;
        else
            if (str[i] != m_blank)
                s += str[i];

    return s;
}

/*!
    \internal
    searches forward/backward in m_maskData for either a separator or a m_blank
*/
int QLineEditModel::findInMask(int pos, bool forward, bool findSeparator, QChar searchChar) const
{
    if (pos >= m_maxLength || pos < 0)
        return -1;

    int end = forward ? m_maxLength : -1;
    int step = forward ? 1 : -1;
    int i = pos;

    while (i != end) {
        if (findSeparator) {
            if (m_maskData[i].separator && m_maskData[i].maskChar == searchChar)
                return i;
        } else {
            if (!m_maskData[i].separator) {
                if (searchChar.isNull())
                    return i;
                else if (isValidInput(searchChar, m_maskData[i].maskChar))
                    return i;
            }
        }
        i += step;
    }
    return -1;
}

void QLineEditModel::internalUndo(int until)
{
    if (!isUndoAvailable())
        return;
    internalDeselect();
    while (m_undoState && m_undoState > until) {
        Command& cmd = m_history[--m_undoState];
        switch (cmd.type) {
        case Insert:
            m_text.remove(cmd.pos, 1);
            m_cursor = cmd.pos;
            break;
        case SetSelection:
            m_selstart = cmd.selStart;
            m_selend = cmd.selEnd;
            m_cursor = cmd.pos;
            break;
        case Remove:
        case RemoveSelection:
            m_text.insert(cmd.pos, cmd.uc);
            m_cursor = cmd.pos + 1;
            break;
        case Delete:
        case DeleteSelection:
            m_text.insert(cmd.pos, cmd.uc);
            m_cursor = cmd.pos;
            break;
        case Separator:
            continue;
        }
        if (until < 0 && m_undoState) {
            Command& next = m_history[m_undoState-1];
            if (next.type != cmd.type && next.type < RemoveSelection
                 && (cmd.type < RemoveSelection || next.type == Separator))
                break;
        }
    }
    m_textDirty = true;
    emitCursorPositionChanged();
}

void QLineEditModel::internalRedo()
{
    if (!isRedoAvailable())
        return;
    internalDeselect();
    while (m_undoState < (int)m_history.size()) {
        Command& cmd = m_history[m_undoState++];
        switch (cmd.type) {
        case Insert:
            m_text.insert(cmd.pos, cmd.uc);
            m_cursor = cmd.pos + 1;
            break;
        case SetSelection:
            m_selstart = cmd.selStart;
            m_selend = cmd.selEnd;
            m_cursor = cmd.pos;
            break;
        case Remove:
        case Delete:
        case RemoveSelection:
        case DeleteSelection:
            m_text.remove(cmd.pos, 1);
            m_selstart = cmd.selStart;
            m_selend = cmd.selEnd;
            m_cursor = cmd.pos;
            break;
        case Separator:
            m_selstart = cmd.selStart;
            m_selend = cmd.selEnd;
            m_cursor = cmd.pos;
            break;
        }
        if (m_undoState < (int)m_history.size()) {
            Command& next = m_history[m_undoState];
            if (next.type != cmd.type && cmd.type < RemoveSelection && next.type != Separator
                 && (next.type < RemoveSelection || cmd.type == Separator))
                break;
        }
    }
    m_textDirty = true;
    emitCursorPositionChanged();
}

/*!
    \internal

    If the current cursor position differs from the last emited cursor
    position, emits cursorPositionChanged().
*/
void QLineEditModel::emitCursorPositionChanged()
{
    if (m_cursor != m_lastCursorPos) {
        const int oldLast = m_lastCursorPos;
        m_lastCursorPos = m_cursor;
        cursorPositionChanged(oldLast, m_cursor);
    }
}

QLineEditEventHelper::QLineEditEventHelper(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), m_model(0)
{
}

QLineEditEventHelper::~QLineEditEventHelper()
{
}


// ### extracted from QLineControl...
void QLineEditEventHelper::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        if (m_model->hasAcceptableInput() || m_model->fixup()) {
            emit accepted();
            emit editingFinished();
        }
        event->ignore();
        return;
    }

    // ### ???
    if (m_model->echoMode() == QLineEdit::PasswordEchoOnEdit
        && !m_model->passwordEchoEditing()
        && !m_model->isReadOnly()
        && !event->text().isEmpty()
        && !(event->modifiers() & Qt::ControlModifier)) {
        // Clear the edit and reset to normal echo mode while editing; the
        // echo mode switches back when the edit loses focus
        // ### resets current content.  dubious code; you can
        // navigate with keys up, down, back, and select(?), but if you press
        // "left" or "right" it clears?
        m_model->updatePasswordEchoEditing(true);
        m_model->clear();
    }

    bool unknown = false;

    if (false) {
    }
#ifndef QT_NO_SHORTCUT
    else if (event == QKeySequence::Undo) {
        if (!m_model->isReadOnly())
            m_model->undo();
    }
    else if (event == QKeySequence::Redo) {
        if (!m_model->isReadOnly())
            m_model->redo();
    }
    else if (event == QKeySequence::SelectAll) {
        m_model->selectAll();
    }
#ifndef QT_NO_CLIPBOARD
    else if (event == QKeySequence::Copy) {
        m_model->copy();
    }
    else if (event == QKeySequence::Paste) {
        if (!m_model->isReadOnly())
            m_model->paste();
    }
    else if (event == QKeySequence::Cut) {
        if (!m_model->isReadOnly()) {
            m_model->copy();
            m_model->del();
        }
    }
    else if (event == QKeySequence::DeleteEndOfLine) {
        if (!m_model->isReadOnly()) {
            m_model->setSelection(m_model->cursorPosition(), m_model->end());
            m_model->copy();
            m_model->del();
        }
    }
#endif //QT_NO_CLIPBOARD
    else if (event == QKeySequence::MoveToStartOfLine || event == QKeySequence::MoveToStartOfBlock) {
        m_model->home(0);
    }
    else if (event == QKeySequence::MoveToEndOfLine || event == QKeySequence::MoveToEndOfBlock) {
        m_model->end(0);
    }
    else if (event == QKeySequence::SelectStartOfLine || event == QKeySequence::SelectStartOfBlock) {
        m_model->home(1);
    }
    else if (event == QKeySequence::SelectEndOfLine || event == QKeySequence::SelectEndOfBlock) {
        m_model->end(1);
    }
    else if (event == QKeySequence::MoveToNextChar) {
        if (m_model->hasSelectedText()) {
            m_model->moveCursor(m_model->selectionEnd(), false);
        } else {
            m_model->cursorForward(0, m_model->layoutDirection() == Qt::LeftToRight ? 1 : -1);
        }
    }
    else if (event == QKeySequence::SelectNextChar) {
        m_model->cursorForward(1, m_model->layoutDirection() == Qt::LeftToRight ? 1 : -1);
    }
    else if (event == QKeySequence::MoveToPreviousChar) {
        if (m_model->hasSelectedText()) {
            m_model->moveCursor(m_model->selectionStart(), false);
        } else {
            m_model->cursorForward(0, m_model->layoutDirection() == Qt::LeftToRight ? -1 : 1);
        }
    }
    else if (event == QKeySequence::SelectPreviousChar) {
        m_model->cursorForward(1, m_model->layoutDirection() == Qt::LeftToRight ? -1 : 1);
    }
    else if (event == QKeySequence::MoveToNextWord) {
        if (m_model->echoMode() == QLineEdit::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordForward(0) : m_model->cursorWordBackward(0);
        else
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->end(0) : m_model->home(0);
    }
    else if (event == QKeySequence::MoveToPreviousWord) {
        if (m_model->echoMode() == QLineEdit::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordBackward(0) : m_model->cursorWordForward(0);
        else if (!m_model->isReadOnly()) {
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->home(0) : m_model->end(0);
        }
    }
    else if (event == QKeySequence::SelectNextWord) {
        if (m_model->echoMode() == QLineEdit::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordForward(1) : m_model->cursorWordBackward(1);
        else
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->end(1) : m_model->home(1);
    }
    else if (event == QKeySequence::SelectPreviousWord) {
        if (m_model->echoMode() == QLineEdit::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordBackward(1) : m_model->cursorWordForward(1);
        else
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->home(1) : m_model->end(1);
    }
    else if (event == QKeySequence::Delete) {
        if (!m_model->isReadOnly())
            m_model->del();
    }
    else if (event == QKeySequence::DeleteEndOfWord) {
        if (!m_model->isReadOnly()) {
            m_model->cursorWordForward(true);
            m_model->del();
        }
    }
    else if (event == QKeySequence::DeleteStartOfWord) {
        if (!m_model->isReadOnly()) {
            m_model->cursorWordBackward(true);
            m_model->del();
        }
    }
#endif // QT_NO_SHORTCUT
    else {
        bool handled = false;
#ifdef Q_WS_MAC
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
            Qt::KeyboardModifiers myModifiers = (event->modifiers() & ~Qt::KeypadModifier);
            if (myModifiers & Qt::ShiftModifier) {
                if (myModifiers == (Qt::ControlModifier|Qt::ShiftModifier)
                        || myModifiers == (Qt::AltModifier|Qt::ShiftModifier)
                        || myModifiers == Qt::ShiftModifier) {

                    event->key() == Qt::Key_Up ? m_model->home(1) : m_model->end(1);
                }
            } else {
                if ((myModifiers == Qt::ControlModifier
                     || myModifiers == Qt::AltModifier
                     || myModifiers == Qt::NoModifier)) {
                    event->key() == Qt::Key_Up ? m_model->home(0) : m_model->end(0);
                }
            }
            handled = true;
        }
#endif
        if (event->modifiers() & Qt::ControlModifier) {
            switch (event->key()) {
            case Qt::Key_Backspace:
                if (!m_model->isReadOnly()) {
                    m_model->cursorWordBackward(true);
                    m_model->del();
                }
                break;
#if defined(Q_WS_X11)
            case Qt::Key_E:
                m_model->end(0);
                break;

            case Qt::Key_U:
                if (!m_model->isReadOnly()) {
                    m_model->setSelection(0, m_model->text().size());
#ifndef QT_NO_CLIPBOARD
                    m_model->copy();
#endif
                    m_model->del();
                }
            break;
#endif
            default:
                if (!handled)
                    unknown = true;
            }
        } else { // ### check for *no* modifier
            switch (event->key()) {
            case Qt::Key_Backspace:
                if (!m_model->isReadOnly()) {
                    m_model->backspace();
                }
                break;

            default:
                if (!handled)
                    unknown = true;
            }
        }
    }

    // ### layout direction :-P
    if (event->key() == Qt::Key_Direction_L || event->key() == Qt::Key_Direction_R) {
        m_model->setLayoutDirection((event->key() == Qt::Key_Direction_L) ? Qt::LeftToRight : Qt::RightToLeft);
        unknown = false;
    }

    if (unknown && !m_model->isReadOnly()) {
        QString t = event->text();
        if (!t.isEmpty() && t.at(0).isPrint()) {
            m_model->insert(t);
            event->accept();
            return;
        }
    }

    if (unknown)
        event->ignore();
    else
        event->accept();
}


QT_END_NAMESPACE

#endif
