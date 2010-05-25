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

class QLineEditModelPrivate;

class QLineEditModel : public QObject
{
    Q_OBJECT
    Q_ENUMS(EchoMode)

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged)

    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(QString selectedText READ selectedText NOTIFY selectionChanged) // ### proper NOTIFY
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionChanged) // ### proper NOTIFY
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionChanged) // ### proper NOTIFY

    Q_PROPERTY(int maximumLength READ maximumLength WRITE setMaximumLength NOTIFY maximumLengthChanged)
#ifndef QT_NO_VALIDATOR
    Q_PROPERTY(QValidator* validator READ validator WRITE setValidator NOTIFY validatorChanged)
#endif
    Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask) // ### NOTIFY
    Q_PROPERTY(bool acceptableInput READ hasAcceptableInput) // ### NOTIFY

    Q_PROPERTY(EchoMode echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)
    Q_PROPERTY(QChar passwordCharacter READ passwordCharacter WRITE setPasswordCharacter NOTIFY passwordCharacterChanged) // ### QML has no QChar

public:
    QLineEditModel(const QString &txt = QString(), QObject *parent = 0);
    virtual ~QLineEditModel();

    QString text() const;
    void setText(const QString &txt);

    QString displayText() const;

    int cursorPosition() const;
    void setCursorPosition(int pos);

    QString selectedText() const;

    int selectionStart() const;
    void setSelectionStart(int selectionStart);

    int selectionEnd() const;
    void setSelectionEnd(int selectionEnd);

    int maximumLength() const;
    void setMaximumLength(int maximumLength);

#ifndef QT_NO_VALIDATOR
    QValidator *validator() const;
    void setValidator(QValidator *);
#endif

    QString inputMask() const;
    void setInputMask(const QString &mask);

    bool hasAcceptableInput() const;

    enum EchoMode { Normal, NoEcho, Password };
    EchoMode echoMode() const;
    void setEchoMode(EchoMode mode);

    QChar passwordCharacter() const;
    void setPasswordCharacter(const QChar &character);

Q_SIGNALS:
    void textChanged(const QString &text);
    void displayTextChanged(const QString &displayText);

    void cursorPositionChanged(int, int); // ### two values?
    void selectionChanged(); // ### Being reused by many properties.

    void maximumLengthChanged(int);
#ifndef QT_NO_VALIDATOR
    void validatorChanged(QValidator *validator);
#endif

    void echoModeChanged(EchoMode echoMode);
    void passwordCharacterChanged(QChar character);

    void textEdited(const QString &); // ### needed?

public:
    // Mask
    int nextMaskBlank(int pos);
    int prevMaskBlank(int pos);

    // Undo/Redo
    bool isUndoAvailable() const;
    bool isRedoAvailable() const;
    void clearUndo();
    bool isModified() const;
    void setModified(bool modified);

    // Selection
    bool allSelected() const;
    bool hasSelectedText() const;
    void setSelection(int start, int length);
    QString textBeforeSelection() const;
    QString textAfterSelection() const;

    // ### Do we need this for our calculations?
    void setLayoutDirection(Qt::LayoutDirection direction);
    Qt::LayoutDirection layoutDirection() const;

    // ###
    int start() const;
    int end() const;

    // ###
    bool fixup();

public Q_SLOTS:
    void moveCursor(int pos, bool mark = false);
    void moveCursorStart(bool mark);
    void moveCursorEnd(bool mark);

    void cursorForward(bool mark, int steps);
    void cursorWordForward(bool mark);
    void cursorWordBackward(bool mark);

    void backspace();
    void del();
    void insert(const QString &);
    void clear();
    void undo();
    void redo();

    void selectAll();
    void deselect();
    void selectWordAtPos(int);
    void removeSelection();

#ifndef QT_NO_CLIPBOARD
    void copy(QClipboard::Mode mode = QClipboard::Clipboard) const;
    void paste();
#endif

protected:
    QLineEditModel(QLineEditModelPrivate &dd, QObject *parent);

private:
    Q_DISABLE_COPY(QLineEditModel)
    Q_DECLARE_PRIVATE(QLineEditModel)
    QLineEditModelPrivate *d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_clipboardChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_deleteSelected())
};

QML_DECLARE_TYPE(QLineEditModel)


// Helper class to do layout calculation
class QLineEditLayoutHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QLineEditModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(qreal cursorX READ cursorX WRITE setCursorX NOTIFY cursorXChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)

public:
    QLineEditLayoutHelper(QObject *parent = 0);

    QLineEditModel *model() { return m_model; }
    void setModel(QLineEditModel *model);

    QFont font() const;
    void setFont(const QFont &font);

    qreal cursorX() const;
    void setCursorX(qreal x);

    // ### Turn into properties or remove.
    int width() const;
    int height() const;
    int ascent() const; // ### ???
    qreal naturalTextWidth() const;

    // ### should 'x' be qreal?
    Q_INVOKABLE int xToPosition(int x, QTextLine::CursorPosition = QTextLine::CursorBetweenCharacters) const;
    Q_INVOKABLE int positionToX(int pos) const;

    // ### Helper for mouse handling
    bool inSelection(int x) const;

Q_SIGNALS:
    void modelChanged(QLineEditModel *);
    void fontChanged(const QFont &);
    void cursorXChanged();

private Q_SLOTS:
    void onDisplayTextChanged(const QString &);

private:
    Q_DISABLE_COPY(QLineEditLayoutHelper)
    QTextLayout textLayout;
    QPointer<QLineEditModel> m_model;
    qreal m_x;

    bool m_xWasSet;

    void updateTextLayout();
};

QML_DECLARE_TYPE(QLineEditLayoutHelper)

// Helper class to do key event handling
class QLineEditEventHelper : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QLineEditModel *model READ model WRITE setModel)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

public:
    QLineEditEventHelper(QDeclarativeItem *parent = 0);
    virtual ~QLineEditEventHelper();

    QLineEditModel *model() { return m_model; }
    void setModel(QLineEditModel *model) { m_model = model; }

    bool isReadOnly() const { return m_readOnly; }
    void setReadOnly(bool enable) { m_readOnly = enable; }

Q_SIGNALS:
    void accepted();
    void editingFinished();

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    QPointer<QLineEditModel> m_model;
    bool m_readOnly;
};

QML_DECLARE_TYPE(QLineEditEventHelper)

#endif // QLINEEDITMODEL_H
