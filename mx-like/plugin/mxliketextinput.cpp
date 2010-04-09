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
#include "mxliketextinput.h"
#include <qdeclarativeinfo.h>

#include <QValidator>
#include <QApplication>
#include <QFontMetrics>
#include <QPainter>

MxLikeTextInput::MxLikeTextInput(QDeclarativeItem* parent)
    : QDeclarativePaintedItem(*(new MxLikeTextInputPrivate), parent)
{
    Q_D(MxLikeTextInput);
    d->init();
}

MxLikeTextInput::~MxLikeTextInput()
{
}

QString MxLikeTextInput::text() const
{
    Q_D(const MxLikeTextInput);
    return d->control->text();
}

void MxLikeTextInput::setText(const QString &s)
{
    Q_D(MxLikeTextInput);
    if(s == text())
        return;
    d->control->setText(s);
    //emit textChanged();
}

QFont MxLikeTextInput::font() const
{
    Q_D(const MxLikeTextInput);
    return d->font;
}

void MxLikeTextInput::setFont(const QFont &font)
{
    Q_D(MxLikeTextInput);
    if (d->font == font)
        return;

    d->font = font;

    d->control->setFont(d->font);
    if(d->cursorItem){
        d->cursorItem->setHeight(QFontMetrics(d->font).height());
        moveCursor();
    }
    updateSize();
    emit fontChanged(d->font);
}

QColor MxLikeTextInput::color() const
{
    Q_D(const MxLikeTextInput);
    return d->color;
}

void MxLikeTextInput::setColor(const QColor &c)
{
    Q_D(MxLikeTextInput);
    d->color = c;
}

QColor MxLikeTextInput::selectionColor() const
{
    Q_D(const MxLikeTextInput);
    return d->selectionColor;
}

void MxLikeTextInput::setSelectionColor(const QColor &color)
{
    Q_D(MxLikeTextInput);
    if (d->selectionColor == color)
        return;

    d->selectionColor = color;
    QPalette p = d->control->palette();
    p.setColor(QPalette::Highlight, d->selectionColor);
    d->control->setPalette(p);
    emit selectionColorChanged(color);
}

QColor MxLikeTextInput::selectedTextColor() const
{
    Q_D(const MxLikeTextInput);
    return d->selectedTextColor;
}

void MxLikeTextInput::setSelectedTextColor(const QColor &color)
{
    Q_D(MxLikeTextInput);
    if (d->selectedTextColor == color)
        return;

    d->selectedTextColor = color;
    QPalette p = d->control->palette();
    p.setColor(QPalette::HighlightedText, d->selectedTextColor);
    d->control->setPalette(p);
    emit selectedTextColorChanged(color);
}

MxLikeTextInput::HAlignment MxLikeTextInput::hAlign() const
{
    Q_D(const MxLikeTextInput);
    return d->hAlign;
}

void MxLikeTextInput::setHAlign(HAlignment align)
{
    Q_D(MxLikeTextInput);
    if(align == d->hAlign)
        return;
    d->hAlign = align;
    updateRect();
    emit horizontalAlignmentChanged(d->hAlign);
}

bool MxLikeTextInput::isReadOnly() const
{
    Q_D(const MxLikeTextInput);
    return d->control->isReadOnly();
}

void MxLikeTextInput::setReadOnly(bool ro)
{
    Q_D(MxLikeTextInput);
    if (d->control->isReadOnly() == ro)
        return;

    d->control->setReadOnly(ro);

    emit readOnlyChanged(ro);
}

int MxLikeTextInput::maxLength() const
{
    Q_D(const MxLikeTextInput);
    return d->control->maxLength();
}

void MxLikeTextInput::setMaxLength(int ml)
{
    Q_D(MxLikeTextInput);
    if (d->control->maxLength() == ml)
        return;

    d->control->setMaxLength(ml);

    emit maximumLengthChanged(ml);
}

bool MxLikeTextInput::isCursorVisible() const
{
    Q_D(const MxLikeTextInput);
    return d->cursorVisible;
}

void MxLikeTextInput::setCursorVisible(bool on)
{
    Q_D(MxLikeTextInput);
    if (d->cursorVisible == on)
        return;
    d->cursorVisible = on;
    d->control->setCursorBlinkPeriod(on?QApplication::cursorFlashTime():0);
    //d->control should emit the cursor update regions
    emit cursorVisibleChanged(d->cursorVisible);
}

int MxLikeTextInput::cursorPosition() const
{
    Q_D(const MxLikeTextInput);
    return d->control->cursor();
}
void MxLikeTextInput::setCursorPosition(int cp)
{
    Q_D(MxLikeTextInput);
    d->control->moveCursor(cp);
}

QRect MxLikeTextInput::cursorRect() const
{
    Q_D(const MxLikeTextInput);
    return d->control->cursorRect();
}

int MxLikeTextInput::selectionStart() const
{
    Q_D(const MxLikeTextInput);
    return d->lastSelectionStart;
}

void MxLikeTextInput::setSelectionStart(int s)
{
    Q_D(MxLikeTextInput);
    if(d->lastSelectionStart == s || s < 0 || s > text().length())
        return;
    d->lastSelectionStart = s;
    d->control->setSelection(s, d->lastSelectionEnd - s);
}

int MxLikeTextInput::selectionEnd() const
{
    Q_D(const MxLikeTextInput);
    return d->lastSelectionEnd;
}

void MxLikeTextInput::setSelectionEnd(int s)
{
    Q_D(MxLikeTextInput);
    if(d->lastSelectionEnd == s || s < 0 || s > text().length())
        return;
    d->lastSelectionEnd = s;
    d->control->setSelection(d->lastSelectionStart, s - d->lastSelectionStart);
}

QString MxLikeTextInput::selectedText() const
{
    Q_D(const MxLikeTextInput);
    return d->control->selectedText();
}

bool MxLikeTextInput::focusOnPress() const
{
    Q_D(const MxLikeTextInput);
    return d->focusOnPress;
}

void MxLikeTextInput::setFocusOnPress(bool b)
{
    Q_D(MxLikeTextInput);
    if (d->focusOnPress == b)
        return;

    d->focusOnPress = b;

    emit focusOnPressChanged(d->focusOnPress);
}

QValidator* MxLikeTextInput::validator() const
{
    Q_D(const MxLikeTextInput);
    //###const cast isn't good, but needed for property system?
    return const_cast<QValidator*>(d->control->validator());
}

void MxLikeTextInput::setValidator(QValidator* v)
{
    Q_D(MxLikeTextInput);
    if (d->control->validator() == v)
        return;

    d->control->setValidator(v);
    if(!d->control->hasAcceptableInput()){
        d->oldValidity = false;
        emit acceptableInputChanged();
    }

    emit validatorChanged();
}

QString MxLikeTextInput::inputMask() const
{
    Q_D(const MxLikeTextInput);
    return d->control->inputMask();
}

void MxLikeTextInput::setInputMask(const QString &im)
{
    Q_D(MxLikeTextInput);
    if (d->control->inputMask() == im)
        return;

    d->control->setInputMask(im);
    emit inputMaskChanged(d->control->inputMask());
}

bool MxLikeTextInput::hasAcceptableInput() const
{
    Q_D(const MxLikeTextInput);
    return d->control->hasAcceptableInput();
}

MxLikeTextInput::EchoMode MxLikeTextInput::echoMode() const
{
    Q_D(const MxLikeTextInput);
    return (MxLikeTextInput::EchoMode)d->control->echoMode();
}

void MxLikeTextInput::setEchoMode(MxLikeTextInput::EchoMode echo)
{
    Q_D(MxLikeTextInput);
    if (echoMode() == echo)
        return;

    d->control->setEchoMode((uint)echo);
    emit echoModeChanged(echoMode());
}

QDeclarativeComponent* MxLikeTextInput::cursorDelegate() const
{
    Q_D(const MxLikeTextInput);
    return d->cursorComponent;
}

void MxLikeTextInput::setCursorDelegate(QDeclarativeComponent* c)
{
    Q_D(MxLikeTextInput);
    if (d->cursorComponent == c)
        return;

    d->cursorComponent = c;
    if(!c){
        //note that the components are owned by something else
        disconnect(d->control, SIGNAL(cursorPositionChanged(int, int)),
                this, SLOT(moveCursor()));
        delete d->cursorItem;
    }else{
        d->startCreatingCursor();
    }

    emit cursorDelegateChanged();
}

void MxLikeTextInputPrivate::startCreatingCursor()
{
    Q_Q(MxLikeTextInput);
    q->connect(control, SIGNAL(cursorPositionChanged(int, int)),
            q, SLOT(moveCursor()));
    if(cursorComponent->isReady()){
        q->createCursor();
    }else if(cursorComponent->isLoading()){
        q->connect(cursorComponent, SIGNAL(statusChanged(int)),
                q, SLOT(createCursor()));
    }else{//isError
        qmlInfo(q) << MxLikeTextInput::tr("Could not load cursor delegate");
        qWarning() << cursorComponent->errors();
    }
}

void MxLikeTextInput::createCursor()
{
    Q_D(MxLikeTextInput);
    if(d->cursorComponent->isError()){
        qmlInfo(this) << tr("Could not load cursor delegate");
        qWarning() << d->cursorComponent->errors();
        return;
    }

    if(!d->cursorComponent->isReady())
        return;

    if(d->cursorItem)
        delete d->cursorItem;
    d->cursorItem = qobject_cast<QDeclarativeItem*>(d->cursorComponent->create());
    if(!d->cursorItem){
        qmlInfo(this) << tr("Could not instantiate cursor delegate");
        //The failed instantiation should print its own error messages
        return;
    }

    d->cursorItem->setParentItem(this);
    d->cursorItem->setX(d->control->cursorToX());
    d->cursorItem->setHeight(d->control->height());
}

void MxLikeTextInput::moveCursor()
{
    Q_D(MxLikeTextInput);
    if(!d->cursorItem)
        return;
    d->cursorItem->setX(d->control->cursorToX() - d->hscroll);
}

int MxLikeTextInput::xToPos(int x)
{
    Q_D(const MxLikeTextInput);
    return d->control->xToPos(x - d->hscroll);
}

void MxLikeTextInputPrivate::focusChanged(bool hasFocus)
{
    Q_Q(MxLikeTextInput);
    focused = hasFocus;
    q->setCursorVisible(hasFocus);
    QDeclarativeItemPrivate::focusChanged(hasFocus);
}

void MxLikeTextInput::keyPressEvent(QKeyEvent* ev)
{
    Q_D(MxLikeTextInput);
    if(((d->control->cursor() == 0 && ev->key() == Qt::Key_Left)
            || (d->control->cursor() == d->control->text().length()
                && ev->key() == Qt::Key_Right))
            && (d->lastSelectionStart == d->lastSelectionEnd)){
        //ignore when moving off the end
        //unless there is a selection, because then moving will do something (deselect)
        ev->ignore();
    }else{
        d->control->processKeyEvent(ev);
    }
    if (!ev->isAccepted())
        QDeclarativePaintedItem::keyPressEvent(ev);
}

void MxLikeTextInput::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MxLikeTextInput);
    if(d->focusOnPress){
        QGraphicsItem *p = parentItem();//###Is there a better way to find my focus scope?
        while(p) {
            if(p->flags() & QGraphicsItem::ItemIsFocusScope){
                p->setFocus();
                break;
            }
            p = p->parentItem();
        }
        setFocus(true);
    }
    d->control->processEvent(event);
}

void MxLikeTextInput::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MxLikeTextInput);
    QWidget *widget = event->widget();
    if (widget && !d->control->isReadOnly() && boundingRect().contains(event->pos()))
        qt_widget_private(widget)->handleSoftwareInputPanel(event->button(), d->focusOnPress);
    d->control->processEvent(event);
}

bool MxLikeTextInput::event(QEvent* ev)
{
    Q_D(MxLikeTextInput);
    //Anything we don't deal with ourselves, pass to the control
    bool handled = false;
    switch(ev->type()){
        case QEvent::KeyPress:
        case QEvent::KeyRelease://###Should the control be doing anything with release?
        case QEvent::GraphicsSceneMousePress:
        case QEvent::GraphicsSceneMouseRelease:
            break;
        default:
            handled = d->control->processEvent(ev);
            if (ev->type() == QEvent::InputMethod)
                updateSize();
    }
    if(!handled)
        return QDeclarativePaintedItem::event(ev);
    return true;
}

void MxLikeTextInput::geometryChanged(const QRectF &newGeometry,
                                  const QRectF &oldGeometry)
{
    if (newGeometry.width() != oldGeometry.width())
        updateSize();
    QDeclarativePaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void MxLikeTextInput::drawContents(QPainter *p, const QRect &r)
{
    Q_D(MxLikeTextInput);
    p->setRenderHint(QPainter::TextAntialiasing, true);
    p->save();
    p->setPen(QPen(d->color));
    int flags = QLineControl::DrawText;
    if(!isReadOnly() && d->cursorVisible && !d->cursorItem)
        flags |= QLineControl::DrawCursor;
    if (d->control->hasSelectedText()){
            flags |= QLineControl::DrawSelections;
    }

    QPoint offset = QPoint(0,0);
    if(d->hAlign != AlignLeft){
        QFontMetrics fm = QFontMetrics(d->font);
        //###Is this using bearing appropriately?
        int minLB = qMax(0, -fm.minLeftBearing());
        int minRB = qMax(0, -fm.minRightBearing());
        int widthUsed = qRound(d->control->naturalTextWidth()) + 1 + minRB;
        int hOffset = 0;
        if(d->hAlign == AlignRight){
            hOffset = width() - widthUsed;
        }else if(d->hAlign == AlignHCenter){
            hOffset = (width() - widthUsed) / 2;
        }
        hOffset -= minLB;
        offset = QPoint(hOffset, 0);
    }
    QRect clipRect = r;
    d->control->draw(p, offset, clipRect, flags);

    p->restore();
}

QVariant MxLikeTextInput::inputMethodQuery(Qt::InputMethodQuery property) const
{
    Q_D(const MxLikeTextInput);
    switch(property) {
    case Qt::ImFont:
        return font();
    case Qt::ImCursorPosition:
        return QVariant(d->control->cursor());
    case Qt::ImSurroundingText:
        return QVariant(text());
    case Qt::ImCurrentSelection:
        return QVariant(selectedText());
    case Qt::ImMaximumTextLength:
        return QVariant(maxLength());
    case Qt::ImAnchorPosition:
        if (d->control->selectionStart() == d->control->selectionEnd())
            return QVariant(d->control->cursor());
        else if (d->control->selectionStart() == d->control->cursor())
            return QVariant(d->control->selectionEnd());
        else
            return QVariant(d->control->selectionStart());
    default:
        return QVariant();
    }
}

void MxLikeTextInput::selectAll()
{
    Q_D(MxLikeTextInput);
    d->control->setSelection(0, d->control->text().length());
}

void MxLikeTextInputPrivate::init()
{
    Q_Q(MxLikeTextInput);
    control->setCursorWidth(1);
    control->setLayoutDirection(Qt::LeftToRight);
    q->setSmooth(smooth);
    q->setAcceptedMouseButtons(Qt::LeftButton);
    q->setFlag(QGraphicsItem::ItemHasNoContents, false);
    q->setFlag(QGraphicsItem::ItemAcceptsInputMethod);
    q->connect(control, SIGNAL(cursorPositionChanged(int,int)),
               q, SLOT(cursorPosChanged()));
    q->connect(control, SIGNAL(selectionChanged()),
               q, SLOT(selectionChanged()));
    q->connect(control, SIGNAL(textChanged(const QString &)),
               q, SLOT(q_textChanged()));
    q->connect(control, SIGNAL(accepted()),
               q, SIGNAL(accepted()));
    q->connect(control, SIGNAL(updateNeeded(QRect)),
               q, SLOT(updateRect(QRect)));
    q->connect(control, SIGNAL(cursorPositionChanged(int,int)),
               q, SLOT(updateRect()));//TODO: Only update rect between pos's
    q->connect(control, SIGNAL(selectionChanged()),
               q, SLOT(updateRect()));//TODO: Only update rect in selection
    //Note that above TODOs probably aren't that big a savings
    q->updateSize();
    oldValidity = control->hasAcceptableInput();
    lastSelectionStart = 0;
    lastSelectionEnd = 0;
    QPalette p = control->palette();
    selectedTextColor = p.color(QPalette::HighlightedText);
    selectionColor = p.color(QPalette::Highlight);
    control->setPasswordCharacter(QChar(0x2219));
}

void MxLikeTextInput::cursorPosChanged()
{
    Q_D(MxLikeTextInput);
    emit cursorPositionChanged();

    if(!d->control->hasSelectedText()){
        if(d->lastSelectionStart != d->control->cursor()){
            d->lastSelectionStart = d->control->cursor();
            emit selectionStartChanged();
        }
        if(d->lastSelectionEnd != d->control->cursor()){
            d->lastSelectionEnd = d->control->cursor();
            emit selectionEndChanged();
        }
    }
}

void MxLikeTextInput::selectionChanged()
{
    Q_D(MxLikeTextInput);
    emit selectedTextChanged();

    if(d->lastSelectionStart != d->control->selectionStart()){
        d->lastSelectionStart = d->control->selectionStart();
        if(d->lastSelectionStart == -1)
            d->lastSelectionStart = d->control->cursor();
        emit selectionStartChanged();
    }
    if(d->lastSelectionEnd != d->control->selectionEnd()){
        d->lastSelectionEnd = d->control->selectionEnd();
        if(d->lastSelectionEnd == -1)
            d->lastSelectionEnd = d->control->cursor();
        emit selectionEndChanged();
    }
}

void MxLikeTextInput::q_textChanged()
{
    Q_D(MxLikeTextInput);
    updateSize();
    emit textChanged();
    if(hasAcceptableInput() != d->oldValidity){
        d->oldValidity = hasAcceptableInput();
        emit acceptableInputChanged();
    }
}

void MxLikeTextInput::updateRect(const QRect &r)
{
    if(r == QRect())
        clearCache();
    else
        dirtyCache(r);
    update();
}

void MxLikeTextInput::updateSize(bool needsRedraw)
{
    Q_D(MxLikeTextInput);
    int w = width();
    int h = height();
    setImplicitHeight(d->control->height());
    int cursorWidth = d->control->cursorWidth();
    if(d->cursorItem)
        cursorWidth = d->cursorItem->width();
    //### Is QFontMetrics too slow?
    QFontMetricsF fm(d->font);
    setImplicitWidth(fm.width(d->control->displayText())+cursorWidth);
    setContentsSize(QSize(width(), height()));//Repaints if changed
    if(w==width() && h==height() && needsRedraw){
        clearCache();
        update();
    }
}

void MxLikeTextInput::moveCursor(int pos, bool mark)
{
    Q_D(MxLikeTextInput);
    d->control->moveCursor(pos,mark);
}
