/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
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
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef MXLIKETEXTINPUT_H
#define MXLIKETEXTINPUT_H

#include <QtDeclarative/private/qdeclarativetext_p.h>
#include <QtDeclarative/private/qdeclarativepainteditem_p.h>
#include <QtDeclarative/private/qdeclarativepainteditem_p_p.h>
#include <QtGui/private/qlinecontrol_p.h>

#include <QGraphicsSceneMouseEvent>
#include <QIntValidator>

class MxLikeTextInputPrivate;
class QValidator;
class Q_DECLARATIVE_EXPORT MxLikeTextInput : public QDeclarativePaintedItem
{
    Q_OBJECT
    Q_ENUMS(HAlignment)
    Q_ENUMS(EchoMode)

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)
    Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor NOTIFY selectedTextColorChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(HAlignment horizontalAlignment READ hAlign WRITE setHAlign NOTIFY horizontalAlignmentChanged)

    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly NOTIFY readOnlyChanged)
    Q_PROPERTY(bool cursorVisible READ isCursorVisible WRITE setCursorVisible NOTIFY cursorVisibleChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(QRect cursorRect READ cursorRect NOTIFY cursorPositionChanged)
    Q_PROPERTY(QDeclarativeComponent *cursorDelegate READ cursorDelegate WRITE setCursorDelegate NOTIFY cursorDelegateChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)
    Q_PROPERTY(QString selectedText READ selectedText NOTIFY selectedTextChanged)

    Q_PROPERTY(int maximumLength READ maxLength WRITE setMaxLength NOTIFY maximumLengthChanged)
    Q_PROPERTY(QValidator* validator READ validator WRITE setValidator NOTIFY validatorChanged)
    Q_PROPERTY(QString inputMask READ inputMask WRITE setInputMask NOTIFY inputMaskChanged)
    Q_PROPERTY(Qt::InputMethodHints inputMethodHints READ inputMethodHints WRITE setInputMethodHints)

    Q_PROPERTY(bool acceptableInput READ hasAcceptableInput NOTIFY acceptableInputChanged)
    Q_PROPERTY(EchoMode echoMode READ echoMode WRITE setEchoMode NOTIFY echoModeChanged)
    Q_PROPERTY(bool focusOnPress READ focusOnPress WRITE setFocusOnPress NOTIFY focusOnPressChanged)

public:
    MxLikeTextInput(QDeclarativeItem* parent=0);
    ~MxLikeTextInput();

    enum EchoMode {//To match QLineEdit::EchoMode
        Normal,
        NoEcho,
        Password,
        PasswordEchoOnEdit
    };

    enum HAlignment {
        AlignLeft = Qt::AlignLeft,
        AlignRight = Qt::AlignRight,
        AlignHCenter = Qt::AlignHCenter
    };

    //### Should we have this function, x based properties,
    //### or copy TextEdit with x instead of QTextCursor?
    Q_INVOKABLE int xToPos(int x);
    Q_INVOKABLE void moveCursor(int pos, bool mark=false);

    QString text() const;
    void setText(const QString &);

    QFont font() const;
    void setFont(const QFont &font);

    QColor color() const;
    void setColor(const QColor &c);

    QColor selectionColor() const;
    void setSelectionColor(const QColor &c);

    QColor selectedTextColor() const;
    void setSelectedTextColor(const QColor &c);

    HAlignment hAlign() const;
    void setHAlign(HAlignment align);

    bool isReadOnly() const;
    void setReadOnly(bool);

    bool isCursorVisible() const;
    void setCursorVisible(bool on);

    int cursorPosition() const;
    void setCursorPosition(int cp);

    QRect cursorRect() const;

    int selectionStart() const;
    void setSelectionStart(int);

    int selectionEnd() const;
    void setSelectionEnd(int);

    QString selectedText() const;

    int maxLength() const;
    void setMaxLength(int ml);

    QValidator * validator() const;
    void setValidator(QValidator* v);

    QString inputMask() const;
    void setInputMask(const QString &im);

    EchoMode echoMode() const;
    void setEchoMode(EchoMode echo);

    QDeclarativeComponent* cursorDelegate() const;
    void setCursorDelegate(QDeclarativeComponent*);

    bool focusOnPress() const;
    void setFocusOnPress(bool);

    bool hasAcceptableInput() const;

    void drawContents(QPainter *p,const QRect &r);
    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;

Q_SIGNALS:
    void textChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();
    void selectedTextChanged();
    void accepted();
    void acceptableInputChanged();
    void colorChanged(const QColor &color);
    void selectionColorChanged(const QColor &color);
    void selectedTextColorChanged(const QColor &color);
    void fontChanged(const QFont &font);
    void horizontalAlignmentChanged(HAlignment alignment);
    void readOnlyChanged(bool isReadOnly);
    void cursorVisibleChanged(bool isCursorVisible);
    void cursorDelegateChanged();
    void maximumLengthChanged(int maximumLength);
    void validatorChanged();
    void inputMaskChanged(const QString &inputMask);
    void echoModeChanged(EchoMode echoMode);
    void focusOnPressChanged(bool focusOnPress);

protected:
    virtual void geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent* ev);
    bool event(QEvent *e);

public Q_SLOTS:
    void selectAll();

private Q_SLOTS:
    void updateSize(bool needsRedraw = true);
    void q_textChanged();
    void selectionChanged();
    void createCursor();
    void moveCursor();
    void cursorPosChanged();
    void updateRect(const QRect &r = QRect());

private:
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), MxLikeTextInput)
};

class MxLikeTextInputPrivate : public QDeclarativePaintedItemPrivate
{
    Q_DECLARE_PUBLIC(MxLikeTextInput)
public:
    MxLikeTextInputPrivate() : control(new QLineControl(QString())),
                 color((QRgb)0), style(QDeclarativeText::Normal),
                 styleColor((QRgb)0), hAlign(MxLikeTextInput::AlignLeft),
                 hscroll(0), oldScroll(0), focused(false), focusOnPress(true),
                 cursorVisible(false)
    {
    }

    ~MxLikeTextInputPrivate()
    {
        delete control;
    }

    void init();
    void startCreatingCursor();
    void focusChanged(bool hasFocus);

    QLineControl* control;

    QFont font;
    QColor  color;
    QColor  selectionColor;
    QColor  selectedTextColor;
    QDeclarativeText::TextStyle style;
    QColor  styleColor;
    MxLikeTextInput::HAlignment hAlign;
    QPointer<QDeclarativeComponent> cursorComponent;
    QPointer<QDeclarativeItem> cursorItem;

    int lastSelectionStart;
    int lastSelectionEnd;
    int oldHeight;
    int oldWidth;
    bool oldValidity;
    int hscroll;
    int oldScroll;
    bool focused;
    bool focusOnPress;
    bool cursorVisible;
};

QML_DECLARE_TYPE(MxLikeTextInput)
#endif
