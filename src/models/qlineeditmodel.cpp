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

#include "qlineeditmodel_p.h"
#include "qlineeditmodel.h"

#ifndef QT_NO_LINEEDIT

#include <QtGui/qclipboard.h>
#include <QtGui/qapplication.h>

QT_BEGIN_NAMESPACE

QLineEditModel::QLineEditModel(const QString &txt, QObject *parent)
    : QObject(parent), d_ptr(new QLineEditModelPrivate(this))
{
    Q_D(QLineEditModel);
    d->init(txt);
}

QLineEditModel::QLineEditModel(QLineEditModelPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{

}

QLineEditModel::~QLineEditModel()
{
    delete d_ptr;
}


QString QLineEditModel::text() const
{
    Q_D(const QLineEditModel);
    QString res = d->maskData ? d->stripString(d->text) : d->text;
    return (res.isNull() ? QString::fromLatin1("") : res);
}

void QLineEditModel::setText(const QString &txt)
{
    Q_D(QLineEditModel);
    d->internalSetText(txt, -1, false);
}

QString QLineEditModel::displayText() const
{
    Q_D(const QLineEditModel);
    return d->textLayout.text();
}

int QLineEditModel::cursorPosition() const
{
    Q_D(const QLineEditModel);
    return d->cursor;
}

void QLineEditModel::setCursorPosition(int pos)
{
    Q_D(QLineEditModel);
    if (pos < 0)
        pos = 0;
    if (pos <= d->text.length())
        moveCursor(pos);
}

QString QLineEditModel::selectedText() const
{
    Q_D(const QLineEditModel);
    if (hasSelectedText())
        return d->text.mid(d->selstart, d->selend - d->selstart);
    return QString();
}

int QLineEditModel::selectionStart() const
{
    Q_D(const QLineEditModel);
    return hasSelectedText() ? d->selstart : -1;
}

void QLineEditModel::setSelectionStart(int s)
{
    Q_D(QLineEditModel);
    if (d->selstart == s)
        return;
    setSelection(s, d->selend);
}

int QLineEditModel::selectionEnd() const
{
    Q_D(const QLineEditModel);
    return hasSelectedText() ? d->selend : -1;
}

void QLineEditModel::setSelectionEnd(int e)
{
    Q_D(QLineEditModel);
    if (d->selend == e)
        return;
    setSelection(d->selstart, e);
}

int QLineEditModel::maximumLength() const
{
    Q_D(const QLineEditModel);
    return d->maxLength;
}

void QLineEditModel::setMaximumLength(int maximumLength)
{
    Q_D(QLineEditModel);
    if (d->maskData)
        return;
    if (d->maxLength != maximumLength) {
        d->maxLength = maximumLength;
        setText(d->text);
        emit maximumLengthChanged(d->maxLength);
    }
}

#ifndef QT_NO_VALIDATOR
QValidator *QLineEditModel::validator() const
{
    Q_D(const QLineEditModel);
    return d->validator;
}

// ### Why this was a const pointer to QValidator?
void QLineEditModel::setValidator(QValidator *v)
{
    Q_D(QLineEditModel);
    if (d->validator != v) {
        d->validator = v;
        emit validatorChanged(d->validator);
    }
}
#endif

QString QLineEditModel::inputMask() const
{
    Q_D(const QLineEditModel);
    return d->maskData ? d->inputMask + QLatin1Char(';') + d->blank : QString();
}

void QLineEditModel::setInputMask(const QString &mask)
{
    Q_D(QLineEditModel);
    d->parseInputMask(mask);
    if (d->maskData)
        moveCursor(nextMaskBlank(0));
}

bool QLineEditModel::hasAcceptableInput() const
{
    Q_D(const QLineEditModel);
    return d->hasAcceptableInput(d->text);
}

QLineEditModel::EchoMode QLineEditModel::echoMode() const
{
    Q_D(const QLineEditModel);
    return EchoMode(d->echoMode);
}

void QLineEditModel::setEchoMode(QLineEditModel::EchoMode mode)
{
    Q_D(QLineEditModel);
    if (EchoMode(d->echoMode) != mode) {
        d->echoMode = mode;
        d->updateDisplayText();
        emit echoModeChanged(EchoMode(d->echoMode));
    }
}

QChar QLineEditModel::passwordCharacter() const
{
    Q_D(const QLineEditModel);
    return d->passwordCharacter;
}

void QLineEditModel::setPasswordCharacter(const QChar &character)
{
    Q_D(QLineEditModel);
    if (d->passwordCharacter != character) {
        d->passwordCharacter = character;
        d->updateDisplayText();
        emit passwordCharacterChanged(character);
    }
}

int QLineEditModel::nextMaskBlank(int pos)
{
    Q_D(QLineEditModel);
    int c = d->findInMask(pos, true, false);
    d->separator |= (c != pos);
    return (c != -1 ?  c : d->maxLength);
}

int QLineEditModel::prevMaskBlank(int pos)
{
    Q_D(QLineEditModel);
    int c = d->findInMask(pos, false, false);
    d->separator |= (c != pos);
    return (c != -1 ? c : 0);
}

bool QLineEditModel::isUndoAvailable() const
{
    Q_D(const QLineEditModel);
    // return !m_readOnly && m_undoState;
    return d->undoState;
}

bool QLineEditModel::isRedoAvailable() const
{
    Q_D(const QLineEditModel);
    // return !m_readOnly && m_undoState < (int)m_history.size();
    return d->undoState < (int)d->history.size();
}

void QLineEditModel::clearUndo()
{
    Q_D(QLineEditModel);
    d->history.clear();
    d->modifiedState = d->undoState = 0;
}

bool QLineEditModel::isModified() const
{
    Q_D(const QLineEditModel);
    return d->modifiedState != d->undoState;
}

void QLineEditModel::setModified(bool modified)
{
    Q_D(QLineEditModel);
    d->modifiedState = modified ? -1 : d->undoState;
}

bool QLineEditModel::allSelected() const
{
    Q_D(const QLineEditModel);
    return !d->text.isEmpty() && d->selstart == 0 && d->selend == (int)d->text.length();
}

bool QLineEditModel::hasSelectedText() const
{
    Q_D(const QLineEditModel);
    return !d->text.isEmpty() && d->selend > d->selstart;
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
    Q_D(QLineEditModel);
    if(start < 0 || start > (int)d->text.length()){
        qWarning("QLineEditModel::setSelection: Invalid start position");
        return;
    }

    if (length > 0) {
        if (start == d->selstart && start + length == d->selend)
            return;
        d->selstart = start;
        d->selend = qMin(start + length, (int)d->text.length());
        d->cursor = d->selend;
    } else {
        if (start == d->selend && start + length == d->selstart)
            return;
        d->selstart = qMax(start + length, 0);
        d->selend = start;
        d->cursor = d->selstart;
    }
    emit selectionChanged();
    d->emitCursorPositionChanged();
}

QString QLineEditModel::textBeforeSelection() const
{
    Q_D(const QLineEditModel);
    if (hasSelectedText())
        return d->text.left(d->selstart);
    return QString();
}

QString QLineEditModel::textAfterSelection() const
{
    Q_D(const QLineEditModel);
    if (hasSelectedText())
        return d->text.mid(d->selend);
    return QString();
}

Qt::LayoutDirection QLineEditModel::layoutDirection() const
{
    Q_D(const QLineEditModel);
    return d->layoutDirection;
}

void QLineEditModel::setLayoutDirection(Qt::LayoutDirection direction)
{
    Q_D(QLineEditModel);
    if (direction != d->layoutDirection) {
        d->layoutDirection = direction;
        d->updateDisplayText();
    }
}

int QLineEditModel::start() const
{
    return 0;
}

int QLineEditModel::end() const
{
    Q_D(const QLineEditModel);
    return d->text.length();
}

/*!
    \internal

    Fixes the current text so that it is valid given any set validators.

    Returns true if the text was changed.  Otherwise returns false.
*/
bool QLineEditModel::fixup() // this function assumes that validate currently returns != Acceptable
{
#ifndef QT_NO_VALIDATOR
    Q_D(QLineEditModel);
    if (d->validator) {
        QString textCopy = d->text;
        int cursorCopy = d->cursor;
        d->validator->fixup(textCopy);
        if (d->validator->validate(textCopy, cursorCopy) == QValidator::Acceptable) {
            if (textCopy != d->text || cursorCopy != d->cursor)
                d->internalSetText(textCopy, cursorCopy);
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
    Q_D(QLineEditModel);
    if (pos != d->cursor) {
        d->separate();
        if (d->maskData)
            pos = pos > d->cursor ? nextMaskBlank(pos) : prevMaskBlank(pos);
    }
    if (mark) {
        int anchor;
        if (d->selend > d->selstart && d->cursor == d->selstart)
            anchor = d->selend;
        else if (d->selend > d->selstart && d->cursor == d->selend)
            anchor = d->selstart;
        else
            anchor = d->cursor;
        d->selstart = qMin(anchor, pos);
        d->selend = qMax(anchor, pos);
        d->updateDisplayText();
    } else {
        d->internalDeselect();
    }
    d->cursor = pos;
    if (mark || d->selDirty) {
        d->selDirty = false;
        emit selectionChanged();
    }
    d->emitCursorPositionChanged();
}

void QLineEditModel::moveCursorStart(bool mark)
{
    moveCursor(0, mark);
}

void QLineEditModel::moveCursorEnd(bool mark)
{
    moveCursor(text().length(), mark);
}

void QLineEditModel::cursorForward(bool mark, int steps)
{
    Q_D(QLineEditModel);
    int c = d->cursor;
    if (steps > 0) {
        while (steps--)
            c = d->textLayout.nextCursorPosition(c);
    } else if (steps < 0) {
        while (steps++)
            c = d->textLayout.previousCursorPosition(c);
    }
    moveCursor(c, mark);
}

void QLineEditModel::cursorWordForward(bool mark)
{
    Q_D(QLineEditModel);
    moveCursor(d->textLayout.nextCursorPosition(d->cursor, QTextLayout::SkipWords), mark);
}

void QLineEditModel::cursorWordBackward(bool mark)
{
    Q_D(QLineEditModel);
    moveCursor(d->textLayout.previousCursorPosition(d->cursor, QTextLayout::SkipWords), mark);
}

/*!
    \internal

    Handles the behavior for the backspace key or function.
    Removes the current selection if there is a selection, otherwise
    removes the character prior to the cursor position.

    \sa del()
*/
void QLineEditModel::backspace()
{
    Q_D(QLineEditModel);
    int priorState = d->undoState;
    if (hasSelectedText()) {
        d->removeSelectedText();
    } else if (d->cursor) {
            --d->cursor;
            if (d->maskData)
                d->cursor = prevMaskBlank(d->cursor);
            QChar uc = d->text.at(d->cursor);
            if (d->cursor > 0 && uc.unicode() >= 0xdc00 && uc.unicode() < 0xe000) {
                // second half of a surrogate, check if we have the first half as well,
                // if yes delete both at once
                uc = d->text.at(d->cursor - 1);
                if (uc.unicode() >= 0xd800 && uc.unicode() < 0xdc00) {
                    d->internalDelete(true);
                    --d->cursor;
                }
            }
            d->internalDelete(true);
    }
    d->finishChange(priorState);
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
    Q_D(QLineEditModel);
    int priorState = d->undoState;
    if (hasSelectedText()) {
        d->removeSelectedText();
    } else {
        int n = d->textLayout.nextCursorPosition(d->cursor) - d->cursor;
        while (n--)
            d->internalDelete();
    }
    d->finishChange(priorState);
}

/*!
    \internal

    Inserts the given \a newText at the current cursor position.
    If there is any selected text it is removed prior to insertion of
    the new text.
*/
void QLineEditModel::insert(const QString &newText)
{
    Q_D(QLineEditModel);
    int priorState = d->undoState;
    d->removeSelectedText();
    d->internalInsert(newText);
    d->finishChange(priorState);
}

/*!
    \internal

    Clears the line control text.
*/
void QLineEditModel::clear()
{
    Q_D(QLineEditModel);
    int priorState = d->undoState;
    d->selstart = 0;
    d->selend = d->text.length();
    d->removeSelectedText();
    d->separate();
    d->finishChange(priorState, /*update*/false, /*edited*/false);
}

void QLineEditModel::undo()
{
    Q_D(QLineEditModel);
    d->internalUndo();
    d->finishChange(-1, true);
}

void QLineEditModel::redo()
{
    Q_D(QLineEditModel);
    d->internalRedo();
    d->finishChange();
}

void QLineEditModel::selectAll()
{
    Q_D(QLineEditModel);
    d->selstart = d->selend = d->cursor = 0;
    moveCursor(d->text.length(), true);
}

void QLineEditModel::deselect()
{
    Q_D(QLineEditModel);
    d->internalDeselect();
    d->finishChange();
}

/*!
    \internal

    Sets the selection to cover the word at the given cursor position.
    The word boundries is defined by the behavior of QTextLayout::SkipWords
    cursor mode.
*/
void QLineEditModel::selectWordAtPos(int cursor)
{
    Q_D(QLineEditModel);
    int c = d->textLayout.previousCursorPosition(cursor, QTextLayout::SkipWords);
    moveCursor(c, false);
    // ## text layout should support end of words.
    int end = d->textLayout.nextCursorPosition(cursor, QTextLayout::SkipWords);
    while (end > cursor && d->text[end-1].isSpace())
        --end;
    moveCursor(end, true);
}

void QLineEditModel::removeSelection()
{
    Q_D(QLineEditModel);
    int priorState = d->undoState;
    d->removeSelectedText();
    d->finishChange(priorState);
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
    Q_D(const QLineEditModel);
    QString t = selectedText();
    if (!t.isEmpty() && d->echoMode == Normal) {
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
    Q_D(QLineEditModel);
    QString clip = QApplication::clipboard()->text(QClipboard::Clipboard);
    if (!clip.isEmpty() || hasSelectedText()) {
        d->separate(); //make it a separate undo/redo command
        insert(clip);
        d->separate();
    }
}

#endif // !QT_NO_CLIPBOARD


QLineEditModelPrivate::QLineEditModelPrivate(QLineEditModel *qq)
    : q_ptr(qq), cursor(0), layoutDirection(Qt::LeftToRight),
      separator(0), dragEnabled(0), echoMode(QLineEditModel::Normal),
      textDirty(0), selDirty(0), validInput(1), deleteAllTimer(0),
      maxLength(32767), lastCursorPos(-1),
      maskData(0), modifiedState(0), undoState(0),
      selstart(0), selend(0), passwordCharacter('*')
{
}

QLineEditModelPrivate::~QLineEditModelPrivate()
{
    delete [] maskData;
}

/*!
    \internal

    Initializes the line control with a starting text value of \a txt.
*/
void QLineEditModelPrivate::init(const QString &txt)
{
    text = txt;
    updateDisplayText();
    cursor = text.length();
}

/*!
    \internal

    Updates the display text based of the current edit text
    If the text has changed will emit displayTextChanged()
*/
void QLineEditModelPrivate::updateDisplayText()
{
    Q_Q(QLineEditModel);
    QString orig = textLayout.text();
    QString str;
    if (echoMode == QLineEditModel::NoEcho)
        str = QString::fromLatin1("");
    else
        str = text;

    if (echoMode == QLineEditModel::Password)
        str.fill(passwordCharacter);

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

    textLayout.setText(str);

    QTextOption option;
    option.setTextDirection(layoutDirection);
    option.setFlags(QTextOption::IncludeTrailingSpaces);
    textLayout.setTextOption(option);

    textLayout.beginLayout();
    QTextLine l = textLayout.createLine();
    textLayout.endLayout();

    if (str != orig)
        emit q->displayTextChanged(str);
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
bool QLineEditModelPrivate::finishChange(int validateFromState, bool update, bool edited)
{
    Q_Q(QLineEditModel);
    Q_UNUSED(update);
    bool lineDirty = selDirty;
    if (textDirty) {
        // do validation
        bool wasValidInput = validInput;
        validInput = true;
#ifndef QT_NO_VALIDATOR
        if (validator) {
            validInput = false;
            QString textCopy = text;
            int cursorCopy = cursor;
            validInput = (validator->validate(textCopy, cursorCopy) != QValidator::Invalid);
            if (validInput) {
                if (text != textCopy) {
                    internalSetText(textCopy, cursorCopy);
                    return true;
                }
                cursor = cursorCopy;
            }
        }
#endif
        if (validateFromState >= 0 && wasValidInput && !validInput) {
            if (transactions.count())
                return false;
            internalUndo(validateFromState);
            history.resize(undoState);
            if (modifiedState > undoState)
                modifiedState = -1;
            validInput = true;
            textDirty = false;
        }
        updateDisplayText();
        lineDirty |= textDirty;
        if (textDirty) {
            textDirty = false;
            QString actualText = q->text();
            if (edited)
                emit q->textEdited(actualText);
            emit q->textChanged(actualText);
        }
    }
    if (selDirty) {
        selDirty = false;
        emit q->selectionChanged();
    }
    emitCursorPositionChanged();
    return true;
}

/*!
    \internal

    An internal function for setting the text of the line control.
*/
void QLineEditModelPrivate::internalSetText(const QString &txt, int pos, bool edited)
{
    internalDeselect();
    QString oldText = text;
    if (maskData) {
        text = maskString(0, txt, true);
        text += clearString(text.length(), maxLength - text.length());
    } else {
        text = txt.isEmpty() ? txt : txt.left(maxLength);
    }
    history.clear();
    modifiedState =  undoState = 0;
    cursor = (pos < 0 || pos > text.length()) ? text.length() : pos;
    textDirty = (oldText != text);
    finishChange(-1, true, edited);
}


/*!
    \internal

    Adds the given \a command to the undo history
    of the line control.  Does not apply the command.
*/
void QLineEditModelPrivate::addCommand(const Command &cmd)
{
    if (separator && undoState && history[undoState - 1].type != Separator) {
        history.resize(undoState + 2);
        history[undoState++] = Command(Separator, cursor, 0, selstart, selend);
    } else {
        history.resize(undoState + 1);
    }
    separator = false;
    history[undoState++] = cmd;
}

/*!
    \internal

    Inserts the given string \a s into the line
    control.

    Also adds the appropriate commands into the undo history.
    This function does not call finishChange(), and may leave the text
    in an invalid state.
*/
void QLineEditModelPrivate::internalInsert(const QString &s)
{
    Q_Q(QLineEditModel);
    if (q->hasSelectedText())
        addCommand(Command(SetSelection, cursor, 0, selstart, selend));
    if (maskData) {
        QString ms = maskString(cursor, s);
        for (int i = 0; i < (int) ms.length(); ++i) {
            addCommand (Command(DeleteSelection, cursor + i, text.at(cursor + i), -1, -1));
            addCommand(Command(Insert, cursor + i, ms.at(i), -1, -1));
        }
        text.replace(cursor, ms.length(), ms);
        cursor += ms.length();
        cursor = q->nextMaskBlank(cursor);
        textDirty = true;
    } else {
        int remaining = maxLength - text.length();
        if (remaining != 0) {
            text.insert(cursor, s.left(remaining));
            for (int i = 0; i < (int) s.left(remaining).length(); ++i)
               addCommand(Command(Insert, cursor++, s.at(i), -1, -1));
            textDirty = true;
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
void QLineEditModelPrivate::internalDelete(bool wasBackspace)
{
    Q_Q(QLineEditModel);
    if (cursor < (int) text.length()) {
        if (q->hasSelectedText())
            addCommand(Command(SetSelection, cursor, 0, selstart, selend));
        addCommand(Command((CommandType)((maskData ? 2 : 0) + (wasBackspace ? Remove : Delete)),
                   cursor, text.at(cursor), -1, -1));
        if (maskData) {
            text.replace(cursor, 1, clearString(cursor, 1));
            addCommand(Command(Insert, cursor, text.at(cursor), -1, -1));
        } else {
            text.remove(cursor, 1);
        }
        textDirty = true;
    }
}

/*!
    \internal

    removes the currently selected text from the line control.

    Also adds the appropriate commands into the undo history.
    This function does not call finishChange(), and may leave the text
    in an invalid state.
*/
void QLineEditModelPrivate::removeSelectedText()
{
    if (selstart < selend && selend <= (int) text.length()) {
        separate();
        int i ;
        addCommand(Command(SetSelection, cursor, 0, selstart, selend));
        if (selstart <= cursor && cursor < selend) {
            // cursor is within the selection. Split up the commands
            // to be able to restore the correct cursor position
            for (i = cursor; i >= selstart; --i)
                addCommand (Command(DeleteSelection, i, text.at(i), -1, 1));
            for (i = selend - 1; i > cursor; --i)
                addCommand (Command(DeleteSelection, i - cursor + selstart - 1, text.at(i), -1, -1));
        } else {
            for (i = selend-1; i >= selstart; --i)
                addCommand (Command(RemoveSelection, i, text.at(i), -1, -1));
        }
        if (maskData) {
            text.replace(selstart, selend - selstart,  clearString(selstart, selend - selstart));
            for (int i = 0; i < selend - selstart; ++i)
                addCommand(Command(Insert, selstart + i, text.at(selstart + i), -1, -1));
        } else {
            text.remove(selstart, selend - selstart);
        }
        if (cursor > selstart)
            cursor -= qMin(cursor, selend) - selstart;
        internalDeselect();
        textDirty = true;
    }
}

/*!
    \internal

    Parses the input mask specified by \a maskFields to generate
    the mask data used to handle input masks.
*/
void QLineEditModelPrivate::parseInputMask(const QString &maskFields)
{
    int delimiter = maskFields.indexOf(QLatin1Char(';'));
    if (maskFields.isEmpty() || delimiter == 0) {
        if (maskData) {
            delete [] maskData;
            maskData = 0;
            maxLength = 32767;
            internalSetText(QString());
        }
        return;
    }

    if (delimiter == -1) {
        blank = QLatin1Char(' ');
        inputMask = maskFields;
    } else {
        inputMask = maskFields.left(delimiter);
        blank = (delimiter + 1 < maskFields.length()) ? maskFields[delimiter + 1] : QLatin1Char(' ');
    }

    // calculate maxLength / maskData length
    maxLength = 0;
    QChar c = 0;
    for (int i=0; i<inputMask.length(); i++) {
        c = inputMask.at(i);
        if (i > 0 && inputMask.at(i-1) == QLatin1Char('\\')) {
            maxLength++;
            continue;
        }
        if (c != QLatin1Char('\\') && c != QLatin1Char('!') &&
             c != QLatin1Char('<') && c != QLatin1Char('>') &&
             c != QLatin1Char('{') && c != QLatin1Char('}') &&
             c != QLatin1Char('[') && c != QLatin1Char(']'))
            maxLength++;
    }

    delete [] maskData;
    maskData = new MaskInputData[maxLength];

    MaskInputData::Casemode m = MaskInputData::NoCaseMode;
    c = 0;
    bool s;
    bool escape = false;
    int index = 0;
    for (int i = 0; i < inputMask.length(); i++) {
        c = inputMask.at(i);
        if (escape) {
            s = true;
            maskData[index].maskChar = c;
            maskData[index].separator = s;
            maskData[index].caseMode = m;
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
                maskData[index].maskChar = c;
                maskData[index].separator = s;
                maskData[index].caseMode = m;
                index++;
            }
        }
    }
    internalSetText(text);
}


/*!
    \internal

    checks if the key is valid compared to the inputMask
*/
bool QLineEditModelPrivate::isValidInput(QChar key, QChar mask) const
{
    switch (mask.unicode()) {
    case 'A':
        if (key.isLetter())
            return true;
        break;
    case 'a':
        if (key.isLetter() || key == blank)
            return true;
        break;
    case 'N':
        if (key.isLetterOrNumber())
            return true;
        break;
    case 'n':
        if (key.isLetterOrNumber() || key == blank)
            return true;
        break;
    case 'X':
        if (key.isPrint())
            return true;
        break;
    case 'x':
        if (key.isPrint() || key == blank)
            return true;
        break;
    case '9':
        if (key.isNumber())
            return true;
        break;
    case '0':
        if (key.isNumber() || key == blank)
            return true;
        break;
    case 'D':
        if (key.isNumber() && key.digitValue() > 0)
            return true;
        break;
    case 'd':
        if ((key.isNumber() && key.digitValue() > 0) || key == blank)
            return true;
        break;
    case '#':
        if (key.isNumber() || key == QLatin1Char('+') || key == QLatin1Char('-') || key == blank)
            return true;
        break;
    case 'B':
        if (key == QLatin1Char('0') || key == QLatin1Char('1'))
            return true;
        break;
    case 'b':
        if (key == QLatin1Char('0') || key == QLatin1Char('1') || key == blank)
            return true;
        break;
    case 'H':
        if (key.isNumber() || (key >= QLatin1Char('a') && key <= QLatin1Char('f')) || (key >= QLatin1Char('A') && key <= QLatin1Char('F')))
            return true;
        break;
    case 'h':
        if (key.isNumber() || (key >= QLatin1Char('a') && key <= QLatin1Char('f')) || (key >= QLatin1Char('A') && key <= QLatin1Char('F')) || key == blank)
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
bool QLineEditModelPrivate::hasAcceptableInput(const QString &str) const
{
#ifndef QT_NO_VALIDATOR
    QString textCopy = str;
    int cursorCopy = cursor;
    if (validator && validator->validate(textCopy, cursorCopy)
        != QValidator::Acceptable)
        return false;
#endif

    if (!maskData)
        return true;

    if (str.length() != maxLength)
        return false;

    for (int i=0; i < maxLength; ++i) {
        if (maskData[i].separator) {
            if (str.at(i) != maskData[i].maskChar)
                return false;
        } else {
            if (!isValidInput(str.at(i), maskData[i].maskChar))
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
QString QLineEditModelPrivate::maskString(uint pos, const QString &str, bool clear) const
{
    if (pos >= (uint)maxLength)
        return QString::fromLatin1("");

    QString fill;
    fill = clear ? clearString(0, maxLength) : text;

    int strIndex = 0;
    QString s = QString::fromLatin1("");
    int i = pos;
    while (i < maxLength) {
        if (strIndex < str.length()) {
            if (maskData[i].separator) {
                s += maskData[i].maskChar;
                if (str[(int)strIndex] == maskData[i].maskChar)
                    strIndex++;
                ++i;
            } else {
                if (isValidInput(str[(int)strIndex], maskData[i].maskChar)) {
                    switch (maskData[i].caseMode) {
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
                        if (str.length() != 1 || i == 0 || (i > 0 && (!maskData[i-1].separator || maskData[i-1].maskChar != str[(int)strIndex]))) {
                            s += fill.mid(i, n-i+1);
                            i = n + 1; // update i to find + 1
                        }
                    } else {
                        // search for valid blank if not
                        n = findInMask(i, true, false, str[(int)strIndex]);
                        if (n != -1) {
                            s += fill.mid(i, n-i);
                            switch (maskData[n].caseMode) {
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
QString QLineEditModelPrivate::clearString(uint pos, uint len) const
{
    if (pos >= (uint)maxLength)
        return QString();

    QString s;
    int end = qMin((uint)maxLength, pos + len);
    for (int i = pos; i < end; ++i)
        if (maskData[i].separator)
            s += maskData[i].maskChar;
        else
            s += blank;

    return s;
}

/*!
    \internal

    Strips blank parts of the input in a QLineEditModel when an inputMask is set,
    separators are still included. Typically "127.0__.0__.1__" becomes "127.0.0.1".
*/
QString QLineEditModelPrivate::stripString(const QString &str) const
{
    if (!maskData)
        return str;

    QString s;
    int end = qMin(maxLength, (int)str.length());
    for (int i = 0; i < end; ++i)
        if (maskData[i].separator)
            s += maskData[i].maskChar;
        else
            if (str[i] != blank)
                s += str[i];

    return s;
}

/*!
    \internal
    searches forward/backward in d->maskData for either a separator or a d->blank
*/
int QLineEditModelPrivate::findInMask(int pos, bool forward, bool findSeparator, QChar searchChar) const
{
    if (pos >= maxLength || pos < 0)
        return -1;

    int end = forward ? maxLength : -1;
    int step = forward ? 1 : -1;
    int i = pos;

    while (i != end) {
        if (findSeparator) {
            if (maskData[i].separator && maskData[i].maskChar == searchChar)
                return i;
        } else {
            if (!maskData[i].separator) {
                if (searchChar.isNull())
                    return i;
                else if (isValidInput(searchChar, maskData[i].maskChar))
                    return i;
            }
        }
        i += step;
    }
    return -1;
}

void QLineEditModelPrivate::internalUndo(int until)
{
    Q_Q(QLineEditModel);
    if (!q->isUndoAvailable())
        return;
    internalDeselect();
    while (undoState && undoState > until) {
        Command& cmd = history[--undoState];
        switch (cmd.type) {
        case Insert:
            text.remove(cmd.pos, 1);
            cursor = cmd.pos;
            break;
        case SetSelection:
            selstart = cmd.selStart;
            selend = cmd.selEnd;
            cursor = cmd.pos;
            break;
        case Remove:
        case RemoveSelection:
            text.insert(cmd.pos, cmd.uc);
            cursor = cmd.pos + 1;
            break;
        case Delete:
        case DeleteSelection:
            text.insert(cmd.pos, cmd.uc);
            cursor = cmd.pos;
            break;
        case Separator:
            continue;
        }
        if (until < 0 && undoState) {
            Command& next = history[undoState-1];
            if (next.type != cmd.type && next.type < RemoveSelection
                 && (cmd.type < RemoveSelection || next.type == Separator))
                break;
        }
    }
    textDirty = true;
    emitCursorPositionChanged();
}

void QLineEditModelPrivate::internalRedo()
{
    Q_Q(QLineEditModel);
    if (!q->isRedoAvailable())
        return;
    internalDeselect();
    while (undoState < (int)history.size()) {
        Command& cmd = history[undoState++];
        switch (cmd.type) {
        case Insert:
            text.insert(cmd.pos, cmd.uc);
            cursor = cmd.pos + 1;
            break;
        case SetSelection:
            selstart = cmd.selStart;
            selend = cmd.selEnd;
            cursor = cmd.pos;
            break;
        case Remove:
        case Delete:
        case RemoveSelection:
        case DeleteSelection:
            text.remove(cmd.pos, 1);
            selstart = cmd.selStart;
            selend = cmd.selEnd;
            cursor = cmd.pos;
            break;
        case Separator:
            selstart = cmd.selStart;
            selend = cmd.selEnd;
            cursor = cmd.pos;
            break;
        }
        if (undoState < (int)history.size()) {
            Command& next = history[undoState];
            if (next.type != cmd.type && cmd.type < RemoveSelection && next.type != Separator
                 && (next.type < RemoveSelection || cmd.type == Separator))
                break;
        }
    }
    textDirty = true;
    emitCursorPositionChanged();
}

/*!
    \internal

    If the current cursor position differs from the last emited cursor
    position, emits cursorPositionChanged().
*/
void QLineEditModelPrivate::emitCursorPositionChanged()
{
    Q_Q(QLineEditModel);
    if (cursor != lastCursorPos) {
        const int oldLast = lastCursorPos;
        lastCursorPos = cursor;
        emit q->cursorPositionChanged(oldLast, cursor);
    }
}

void QLineEditModelPrivate::_q_clipboardChanged()
{
}

void QLineEditModelPrivate::_q_deleteSelected()
{
    Q_Q(QLineEditModel);
    if (!q->hasSelectedText())
        return;

    int priorState = undoState;
    removeSelectedText();
    separate();
    finishChange(priorState);
}

////////////////////////////////////

QLineEditLayoutHelper::QLineEditLayoutHelper(QObject *parent)
    : QObject(parent), m_model(0), m_x(0), m_xWasSet(false)
{
    updateTextLayout();
}

void QLineEditLayoutHelper::setModel(QLineEditModel *model)
{
    if (m_model == model)
        return;

    // Disconnect from old model if there was one
    QLineEditModel *oldModel = m_model;
    if (oldModel) {
        textLayout.setText(QString());
        model->disconnect(this);
    }

    m_model = model;

    // Connect to new model if there is one
    if (model) {
        textLayout.setText(model->displayText());

        // ### reset x?
        if (m_xWasSet)
            model->setCursorPosition(xToPosition(m_x));

        QObject::connect(model, SIGNAL(displayTextChanged(const QString &)),
                         this, SLOT(onDisplayTextChanged(const QString &)));
        QObject::connect(model, SIGNAL(cursorPositionChanged(int, int)),
                         this, SIGNAL(cursorXChanged()));
    }

    emit modelChanged(model);
}

QFont QLineEditLayoutHelper::font() const
{
    return textLayout.font();
}

void QLineEditLayoutHelper::setFont(const QFont &font)
{
    if (textLayout.font() == font)
        return;

    textLayout.setFont(font);

    qreal oldX = cursorX();
    updateTextLayout();
    emit fontChanged(font);

    // CursorX changed due to a font change that we just did
    if (!qFuzzyCompare(oldX, cursorX()))
        emit cursorXChanged();
}

qreal QLineEditLayoutHelper::cursorX() const
{
    if (m_model)
        return positionToX(m_model->cursorPosition());

    return 0;
}

// ### Maybe we do not want to have setter for cursorX?
void QLineEditLayoutHelper::setCursorX(qreal x)
{
    if (qFuzzyCompare(x, m_x))
        return;

    // Used to store the value for the case of 'cursorX' being set
    // before the model.
    m_x = x;
    m_xWasSet = true;


    if (m_model)
        m_model->setCursorPosition(xToPosition(x));
    emit cursorXChanged();
}

bool QLineEditLayoutHelper::inSelection(int x) const
{
    if (!m_model || m_model->selectionStart() >= m_model->selectionEnd())
        return false;

    int pos = xToPosition(x, QTextLine::CursorOnCharacter);
    return pos >= m_model->selectionStart() && pos < m_model->selectionEnd();
}

int QLineEditLayoutHelper::width() const
{
    return qRound(textLayout.lineAt(0).width()) + 1;
}

int QLineEditLayoutHelper::height() const
{
    return qRound(textLayout.lineAt(0).height()) + 1;
}

int QLineEditLayoutHelper::ascent() const
{
    return qRound(textLayout.lineAt(0).ascent());
}

qreal QLineEditLayoutHelper::naturalTextWidth() const
{
    return textLayout.lineAt(0).naturalTextWidth();
}

/*!
    \internal

    Returns the cursor position of the given \a x pixel value in relation
    to the displayed text.  The given \a betweenOrOn specified what kind
    of cursor position is requested.
*/
int QLineEditLayoutHelper::xToPosition(int x, QTextLine::CursorPosition betweenOrOn) const
{
    if (textLayout.text().isEmpty())
        return 0;
    return textLayout.lineAt(0).xToCursor(x, betweenOrOn);
}

int QLineEditLayoutHelper::positionToX(int pos) const
{
    if (textLayout.text().isEmpty())
        return 0;
    return textLayout.lineAt(0).cursorToX(pos);
}

void QLineEditLayoutHelper::onDisplayTextChanged(const QString &displayText)
{
    qreal oldX = cursorX();
    textLayout.setText(displayText);
    updateTextLayout();
    if (!qFuzzyCompare(oldX, cursorX()))
        emit cursorXChanged();
}

void QLineEditLayoutHelper::updateTextLayout()
{
    textLayout.clearLayout();

    QTextOption option;
    if (m_model)
        option.setTextDirection(m_model->layoutDirection());
    option.setFlags(QTextOption::IncludeTrailingSpaces);
    textLayout.setTextOption(option);

    textLayout.beginLayout();
    QTextLine l = textLayout.createLine();
    textLayout.endLayout();
}


////////////////////////////////////

QLineEditEventHelper::QLineEditEventHelper(QDeclarativeItem *parent)
    : QDeclarativeItem(parent), m_model(0), m_readOnly(false)
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

    bool unknown = false;

    if (false) {
    }
#ifndef QT_NO_SHORTCUT
    else if (event == QKeySequence::Undo) {
        if (!m_readOnly)
            m_model->undo();
    }
    else if (event == QKeySequence::Redo) {
        if (!m_readOnly)
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
        if (!m_readOnly)
            m_model->paste();
    }
    else if (event == QKeySequence::Cut) {
        if (!m_readOnly) {
            m_model->copy();
            m_model->del();
        }
    }
    else if (event == QKeySequence::DeleteEndOfLine) {
        if (!m_readOnly) {
            m_model->setSelection(m_model->cursorPosition(), m_model->end());
            m_model->copy();
            m_model->del();
        }
    }
#endif //QT_NO_CLIPBOARD
    else if (event == QKeySequence::MoveToStartOfLine || event == QKeySequence::MoveToStartOfBlock) {
        m_model->moveCursorStart(0);
    }
    else if (event == QKeySequence::MoveToEndOfLine || event == QKeySequence::MoveToEndOfBlock) {
        m_model->moveCursorEnd(0);
    }
    else if (event == QKeySequence::SelectStartOfLine || event == QKeySequence::SelectStartOfBlock) {
        m_model->moveCursorStart(1);
    }
    else if (event == QKeySequence::SelectEndOfLine || event == QKeySequence::SelectEndOfBlock) {
        m_model->moveCursorEnd(1);
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
        if (m_model->echoMode() == QLineEditModel::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordForward(0) : m_model->cursorWordBackward(0);
        else
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->moveCursorEnd(0) : m_model->moveCursorStart(0);
    }
    else if (event == QKeySequence::MoveToPreviousWord) {
        if (m_model->echoMode() == QLineEditModel::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordBackward(0) : m_model->cursorWordForward(0);
        else if (!m_readOnly) {
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->moveCursorStart(0) : m_model->moveCursorEnd(0);
        }
    }
    else if (event == QKeySequence::SelectNextWord) {
        if (m_model->echoMode() == QLineEditModel::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordForward(1) : m_model->cursorWordBackward(1);
        else
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->moveCursorEnd(1) : m_model->moveCursorStart(1);
    }
    else if (event == QKeySequence::SelectPreviousWord) {
        if (m_model->echoMode() == QLineEditModel::Normal)
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->cursorWordBackward(1) : m_model->cursorWordForward(1);
        else
            m_model->layoutDirection() == Qt::LeftToRight ? m_model->moveCursorStart(1) : m_model->moveCursorEnd(1);
    }
    else if (event == QKeySequence::Delete) {
        if (!m_readOnly)
            m_model->del();
    }
    else if (event == QKeySequence::DeleteEndOfWord) {
        if (!m_readOnly) {
            m_model->cursorWordForward(true);
            m_model->del();
        }
    }
    else if (event == QKeySequence::DeleteStartOfWord) {
        if (!m_readOnly) {
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

                    event->key() == Qt::Key_Up ? m_model->moveCursorStart(1) : m_model->moveCursorEnd(1);
                }
            } else {
                if ((myModifiers == Qt::ControlModifier
                     || myModifiers == Qt::AltModifier
                     || myModifiers == Qt::NoModifier)) {
                    event->key() == Qt::Key_Up ? m_model->moveCursorStart(0) : m_model->moveCursorEnd(0);
                }
            }
            handled = true;
        }
#endif
        if (event->modifiers() & Qt::ControlModifier) {
            switch (event->key()) {
            case Qt::Key_Backspace:
                if (!m_readOnly) {
                    m_model->cursorWordBackward(true);
                    m_model->del();
                }
                break;
#if defined(Q_WS_X11)
            case Qt::Key_E:
                m_model->moveCursorEnd(0);
                break;

            case Qt::Key_U:
                if (!m_readOnly) {
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
                if (!m_readOnly) {
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

    if (unknown && !m_readOnly) {
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

// For private slots
#include "moc_qlineeditmodel.cpp"

#endif
