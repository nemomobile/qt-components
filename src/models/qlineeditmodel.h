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

#ifndef QLINEEDITMODEL_H
#define QLINEEDITMODEL_H

#include "QtCore/qglobal.h"

#include "private/qwidget_p.h"
#include "QtGui/qlineedit.h"
#include "QtGui/qtextlayout.h"
#include "QtGui/qstyleoption.h"
#include "QtCore/qpointer.h"
#include "QtGui/qlineedit.h"
#include "QtGui/qclipboard.h"
#include "QtCore/qpoint.h"

#include "QtDeclarative/qdeclarativeitem.h"


class QLineEditModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int maxLength READ maxLength WRITE setMaxLength)
    Q_PROPERTY(QChar passwordCharacter READ passwordCharacter WRITE setPasswordCharacter)

    // ### TextLayout stuff
    Q_PROPERTY(qreal cursorX READ cursorToX WRITE setCursorX_helper NOTIFY cursorPositionChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont)
    // ### end of TextLayout stuff

public:
    QLineEditModel(const QString &txt = QString())
        : m_cursor(0), m_layoutDirection(Qt::LeftToRight),
        m_separator(0), m_readOnly(0),
        m_dragEnabled(0), m_echoMode(0), m_textDirty(0), m_selDirty(0),
        m_validInput(1), m_deleteAllTimer(0),
        m_ascent(0), m_maxLength(32767), m_lastCursorPos(-1),
        m_tripleClickTimer(0), m_maskData(0), m_modifiedState(0), m_undoState(0),
        m_selstart(0), m_selend(0), m_passwordEchoEditing(false)
    {
        init(txt);
    }

    ~QLineEditModel()
    {
        delete [] m_maskData;
    }

    int nextMaskBlank(int pos);
    int prevMaskBlank(int pos);

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;
    void clearUndo();
    bool isModified() const;
    void setModified(bool modified);

    bool allSelected() const;
    bool hasSelectedText() const;

    // ### TextLayout stuff
    int width() const;
    int height() const;
    int ascent() const;
    qreal naturalTextWidth() const;

    int xToPos(int x, QTextLine::CursorPosition = QTextLine::CursorBetweenCharacters) const;

    void setCursorX_helper(int x) {
        setCursorPosition(xToPos(x));
    }

    qreal cursorToX(int cursor) const;
    qreal cursorToX() const;

    Qt::LayoutDirection layoutDirection() const;
    void setLayoutDirection(Qt::LayoutDirection direction);
    void setFont(const QFont &font);
    QFont font() const;
    // ### end of TextLayout stuff

    void setSelection(int start, int length);

    QString selectedText() const;
    QString textBeforeSelection() const;
    QString textAfterSelection() const;

    int selectionStart() const;
    int selectionEnd() const;
    bool inSelection(int x) const;

    void removeSelection();

    int start() const;
    int end() const;

#ifndef QT_NO_CLIPBOARD
    void copy(QClipboard::Mode mode = QClipboard::Clipboard) const;
    void paste();
#endif

    int preeditCursor() const;

    // ### needed?
    bool isReadOnly() const;
    void setReadOnly(bool enable);

    QString text() const;
    void setText(const QString &txt);

    QString displayText() const;

    // ### enum
    uint echoMode() const;
    void setEchoMode(uint mode);

    void setMaxLength(int maxLength);
    int maxLength() const;

#ifndef QT_NO_VALIDATOR
    const QValidator *validator() const;
    void setValidator(const QValidator *);
#endif

    void setCursorPosition(int pos);
    int cursorPosition() const;

    bool hasAcceptableInput() const;
    bool fixup();

    QString inputMask() const;
    void setInputMask(const QString &mask);

    void updatePasswordEchoEditing(bool editing);
    bool passwordEchoEditing() const;

    QChar passwordCharacter() const;
    void setPasswordCharacter(const QChar &character);

    QString cancelText() const;
    void setCancelText(const QString &text);

public Q_SLOTS:
    void moveCursor(int pos, bool mark = false);
    void cursorForward(bool mark, int steps);
    void cursorWordForward(bool mark);
    void cursorWordBackward(bool mark);
    void home(bool mark);
    void end(bool mark);

    void backspace();
    void del();
    void deselect();
    void selectAll();
    void insert(const QString &);
    void clear();
    void undo();
    void redo();
    void selectWordAtPos(int);

private:
    void init(const QString &txt);
    void removeSelectedText();
    void internalSetText(const QString &txt, int pos = -1, bool edited = true);
    void updateDisplayText();

    void internalInsert(const QString &s);
    void internalDelete(bool wasBackspace = false);
    void internalRemove(int pos);

    inline void internalDeselect()
    {
        m_selDirty |= (m_selend > m_selstart);
        m_selstart = m_selend = 0;
    }

    void internalUndo(int until = -1);
    void internalRedo();

    QString m_text;
    int m_cursor;
    int m_preeditCursor;
    Qt::LayoutDirection m_layoutDirection;
    uint m_separator : 1;
    uint m_readOnly : 1;
    uint m_dragEnabled : 1;
    uint m_echoMode : 2;
    uint m_textDirty : 1;
    uint m_selDirty : 1;
    uint m_validInput : 1;
    int m_deleteAllTimer;
    int m_ascent;
    int m_maxLength;
    int m_lastCursorPos;
    QList<int> m_transactions;
    QPoint m_tripleClick;
    int m_tripleClickTimer;
    QString m_cancelText;

    void emitCursorPositionChanged();

    bool finishChange(int validateFromState = -1, bool update = false, bool edited = true);

#ifndef QT_NO_VALIDATOR
    QPointer<QValidator> m_validator;
#endif

    struct MaskInputData {
        enum Casemode { NoCaseMode, Upper, Lower };
        QChar maskChar; // either the separator char or the inputmask
        bool separator;
        Casemode caseMode;
    };
    QString m_inputMask;
    QChar m_blank;
    MaskInputData *m_maskData;

    // undo/redo handling
    enum CommandType { Separator, Insert, Remove, Delete, RemoveSelection, DeleteSelection, SetSelection };
    struct Command {
        inline Command() {}
        inline Command(CommandType t, int p, QChar c, int ss, int se) : type(t),uc(c),pos(p),selStart(ss),selEnd(se) {}
        uint type : 4;
        QChar uc;
        int pos, selStart, selEnd;
    };
    int m_modifiedState;
    int m_undoState;
    QVector<Command> m_history;
    void addCommand(const Command& cmd);

    inline void separate() { m_separator = true; }

    // selection
    int m_selstart;
    int m_selend;

    // masking
    void parseInputMask(const QString &maskFields);
    bool isValidInput(QChar key, QChar mask) const;
    bool hasAcceptableInput(const QString &text) const;
    QString maskString(uint pos, const QString &str, bool clear = false) const;
    QString clearString(uint pos, uint len) const;
    QString stripString(const QString &str) const;
    int findInMask(int pos, bool forward, bool findSeparator, QChar searchChar = QChar()) const;

    // complex text layout
    QTextLayout m_textLayout;

    bool m_passwordEchoEditing;
    QChar m_passwordCharacter;

Q_SIGNALS:
    void cursorPositionChanged(int, int);
    void selectionChanged();

    void displayTextChanged(const QString &);
    void textChanged(const QString &);
    void textEdited(const QString &);

private Q_SLOTS:
    void _q_clipboardChanged();
    void _q_deleteSelected();

};

inline int QLineEditModel::nextMaskBlank(int pos)
{
    int c = findInMask(pos, true, false);
    m_separator |= (c != pos);
    return (c != -1 ?  c : m_maxLength);
}

inline int QLineEditModel::prevMaskBlank(int pos)
{
    int c = findInMask(pos, false, false);
    m_separator |= (c != pos);
    return (c != -1 ? c : 0);
}

inline bool QLineEditModel::isUndoAvailable() const
{ 
    return !m_readOnly && m_undoState;
}

inline bool QLineEditModel::isRedoAvailable() const
{
    return !m_readOnly && m_undoState < (int)m_history.size();
}

inline void QLineEditModel::clearUndo()
{
    m_history.clear();
    m_modifiedState = m_undoState = 0;
}

inline bool QLineEditModel::isModified() const
{
    return m_modifiedState != m_undoState;
}

inline void QLineEditModel::setModified(bool modified)
{
    m_modifiedState = modified ? -1 : m_undoState;
}

inline bool QLineEditModel::allSelected() const
{
    return !m_text.isEmpty() && m_selstart == 0 && m_selend == (int)m_text.length();
}

inline bool QLineEditModel::hasSelectedText() const
{
    return !m_text.isEmpty() && m_selend > m_selstart;
}

inline int QLineEditModel::width() const
{
    return qRound(m_textLayout.lineAt(0).width()) + 1;
}

inline qreal QLineEditModel::naturalTextWidth() const
{
    return m_textLayout.lineAt(0).naturalTextWidth();
}

inline int QLineEditModel::height() const
{
    return qRound(m_textLayout.lineAt(0).height()) + 1;
}

inline int QLineEditModel::ascent() const
{
    return m_ascent;
}

inline QString QLineEditModel::selectedText() const
{
    if (hasSelectedText())
        return m_text.mid(m_selstart, m_selend - m_selstart);
    return QString();
}

inline QString QLineEditModel::textBeforeSelection() const
{
    if (hasSelectedText())
        return m_text.left(m_selstart);
    return QString();
}

inline QString QLineEditModel::textAfterSelection() const
{
    if (hasSelectedText())
        return m_text.mid(m_selend);
    return QString();
}

inline int QLineEditModel::selectionStart() const
{
    return hasSelectedText() ? m_selstart : -1;
}

inline int QLineEditModel::selectionEnd() const
{
    return hasSelectedText() ? m_selend : -1;
}

inline int QLineEditModel::start() const
{
    return 0;
}

inline int QLineEditModel::end() const
{
    return m_text.length();
}

inline void QLineEditModel::removeSelection()
{
    int priorState = m_undoState;
    removeSelectedText();
    finishChange(priorState);
}

inline bool QLineEditModel::inSelection(int x) const
{
    if (m_selstart >= m_selend)
        return false;
    int pos = xToPos(x, QTextLine::CursorOnCharacter);
    return pos >= m_selstart && pos < m_selend;
}

inline int QLineEditModel::preeditCursor() const
{
    return m_preeditCursor;
}

inline void QLineEditModel::cursorForward(bool mark, int steps)
{
    int c = m_cursor;
    if (steps > 0) {
        while (steps--)
            c = m_textLayout.nextCursorPosition(c);
    } else if (steps < 0) {
        while (steps++)
            c = m_textLayout.previousCursorPosition(c);
    }
    moveCursor(c, mark);
}

inline void QLineEditModel::cursorWordForward(bool mark)
{
    moveCursor(m_textLayout.nextCursorPosition(m_cursor, QTextLayout::SkipWords), mark);
}

inline void QLineEditModel::home(bool mark)
{
    moveCursor(0, mark);
}

inline void QLineEditModel::end(bool mark)
{
    moveCursor(text().length(), mark);
}

inline void QLineEditModel::cursorWordBackward(bool mark)
{
    moveCursor(m_textLayout.previousCursorPosition(m_cursor, QTextLayout::SkipWords), mark);
}

inline qreal QLineEditModel::cursorToX(int cursor) const
{
    return m_textLayout.lineAt(0).cursorToX(cursor);
}

inline qreal QLineEditModel::cursorToX() const
{
    return cursorToX(m_cursor);
}

inline bool QLineEditModel::isReadOnly() const
{
    return m_readOnly;
}

inline void QLineEditModel::setReadOnly(bool enable)
{ 
    m_readOnly = enable;
}

inline QString QLineEditModel::text() const
{
    QString res = m_maskData ? stripString(m_text) : m_text;
    return (res.isNull() ? QString::fromLatin1("") : res);
}

inline void QLineEditModel::setText(const QString &txt)
{
    internalSetText(txt, -1, false);
}

inline QString QLineEditModel::displayText() const
{
    return m_textLayout.text();
}

inline void QLineEditModel::deselect()
{
    internalDeselect();
    finishChange();
}

inline void QLineEditModel::selectAll()
{
    m_selstart = m_selend = m_cursor = 0;
    moveCursor(m_text.length(), true);
}

inline void QLineEditModel::undo()
{
    internalUndo();
    finishChange(-1, true);
}

inline void QLineEditModel::redo()
{
    internalRedo();
    finishChange();
}

inline uint QLineEditModel::echoMode() const
{
    return m_echoMode;
}

inline void QLineEditModel::setEchoMode(uint mode)
{
    m_echoMode = mode;
    m_passwordEchoEditing = false;
    updateDisplayText();
}

inline void QLineEditModel::setMaxLength(int maxLength)
{
    if (m_maskData)
        return;
    m_maxLength = maxLength;
    setText(m_text);
}

inline int QLineEditModel::maxLength() const
{
    return m_maxLength;
}

#ifndef QT_NO_VALIDATOR
inline const QValidator *QLineEditModel::validator() const
{
    return m_validator;
}

inline void QLineEditModel::setValidator(const QValidator *v)
{
    m_validator = const_cast<QValidator*>(v);
}
#endif

inline void QLineEditModel::setCursorPosition(int pos)
{
    if (pos < 0)
        pos = 0;
    if (pos <= m_text.length())
        moveCursor(pos);
}

inline int QLineEditModel::cursorPosition() const
{
    return m_cursor;
}

inline bool QLineEditModel::hasAcceptableInput() const
{
    return hasAcceptableInput(m_text);
}

inline QString QLineEditModel::inputMask() const
{
    return m_maskData ? m_inputMask + QLatin1Char(';') + m_blank : QString();
}

inline void QLineEditModel::setInputMask(const QString &mask)
{
    parseInputMask(mask);
    if (m_maskData)
        moveCursor(nextMaskBlank(0));
}

inline bool QLineEditModel::passwordEchoEditing() const
{
    return m_passwordEchoEditing;
}

inline QChar QLineEditModel::passwordCharacter() const
{
    return m_passwordCharacter;
}

inline void QLineEditModel::setPasswordCharacter(const QChar &character)
{
    m_passwordCharacter = character;
    updateDisplayText();
}

inline Qt::LayoutDirection QLineEditModel::layoutDirection() const
{
    return m_layoutDirection;
}

inline void QLineEditModel::setLayoutDirection(Qt::LayoutDirection direction)
{
    if (direction != m_layoutDirection) {
        m_layoutDirection = direction;
        updateDisplayText();
    }
}

inline void QLineEditModel::setFont(const QFont &font)
{
    m_textLayout.setFont(font);
    updateDisplayText();
}

inline QFont QLineEditModel::font() const
{
    return m_textLayout.font();
}

inline QString QLineEditModel::cancelText() const
{
    return m_cancelText;
}

inline void QLineEditModel::setCancelText(const QString &text)
{
    m_cancelText = text;
}

QML_DECLARE_TYPE(QLineEditModel);


class QLineEditEventHelper : public QDeclarativeItem
{
    Q_OBJECT;

    Q_PROPERTY(QLineEditModel *model READ model WRITE setModel);

public:
    QLineEditEventHelper(QDeclarativeItem *parent = 0);
    virtual ~QLineEditEventHelper();

    QLineEditModel *model() { return m_model; }
    void setModel(QLineEditModel *model) { m_model = model; }

Q_SIGNALS:
    void accepted();
    void editingFinished();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    QPointer<QLineEditModel> m_model;
};

QML_DECLARE_TYPE(QLineEditEventHelper);

#endif // QLINEEDITMODEL_H
