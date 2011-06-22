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

#include "mdeclarativemaskeditem.h"

#include <QEvent>
#include <QPainter>
#include <QScopedPointer>
#include <QGraphicsEffect>
#include <QDeclarativeComponent>


class MaskEffect : public QGraphicsEffect
{
    Q_OBJECT

public:
    MaskEffect(QObject *parent = 0);

    void setMask(QDeclarativeItem *mask);

protected:
    void draw(QPainter *painter);

private:
    QPixmap m_buffer;
    QScopedPointer<QDeclarativeItem> m_item;
};


MaskEffect::MaskEffect(QObject *parent)
    : QGraphicsEffect(parent)
{

}

void MaskEffect::setMask(QDeclarativeItem *mask)
{
    if (!m_item.isNull()) {
        disconnect(m_item.data(), SIGNAL(widthChanged()), this, SLOT(update()));
        disconnect(m_item.data(), SIGNAL(heightChanged()), this, SLOT(update()));
    }

    m_item.reset(mask);
    update();

    if (!m_item.isNull()) {
        connect(m_item.data(), SIGNAL(widthChanged()), this, SLOT(update()));
        connect(m_item.data(), SIGNAL(heightChanged()), this, SLOT(update()));
    }
}

void MaskEffect::draw(QPainter *painter)
{
    if (m_item.isNull()) {
        drawSource(painter);
        return;
    }

    QPoint offset;
    const QPixmap &pixmap = sourcePixmap(Qt::LogicalCoordinates,
                                         &offset, QGraphicsEffect::NoPad);

    if (pixmap.isNull())
        return;

    if (pixmap.size() != m_buffer.size())
        m_buffer = pixmap;

    QPainter p(&m_buffer);

    // clear buffer and set mask
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(0, 0, m_buffer.width(), m_buffer.height(), Qt::transparent);
    m_item->paint(&p, 0, 0);

    // compose pixmap with mask in buffer
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.drawPixmap(0, 0, pixmap);

    // draw composed buffer
    painter->drawPixmap(offset, m_buffer);
}


/*!
  \qmlclass MaskedItem MDeclarativeMaskedItem
  \brief The MaskedItem element allows you to compose an Item with an alpha mask.
  \inherits Item

  The MaskedItem element reveals portion of your picture or item by composing this
  item with a mask element. The mask element will be used as an alpha mask.

  \section1 Usage Example

  The following example shows how to apply an alpha mask in a rectangle.

  \qml
  MaskedItem {
      width: 100
      height: 100

      mask: Image {
          source: "mask.png"
      }

      Rectangle {
          anchors.fill: parent
          color: "red"
      }
  }
  \endqml

  \section1 Performance Issues

  Internally MaskedItem needs to cache their children in a pixmap in
  order to compose the final result with the mask. So, some cautions are
  needed while using this item:

  1. MaskedItem is clipped by default. It's recommended to not set clip
  to false, since this may lead to high memory consumption depending on
  the children geometries; behind the scenes, a pixmap with the size of
  the final bounding rect is needed. So clipping guarantees that this
  final bounding rect will not be greater than the element itself.

  2. Avoid resizing the MaskedItem element in animations. When the element
  is resized a new pixmap is created according to the new size, and this
  will have performance impact. You can resize the children though, if clip
  is not changed.

  3. Try to minimize children updates (like child resize, move, ...), this
  will reduce cache repaints. You can change mask item size with no additional
  cost though.

  \section1 Limitations

  1. Currently just the mask element contents is used to create the alpha mask.
  So do not place any children inside its declaration, as they will not be used.
*/

MDeclarativeMaskedItem::MDeclarativeMaskedItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
      m_maskComponent(0)
{
    // XXX: this is a workaround to avoid a bug in
    // QGraphicsEffect that keeps dirty regions on
    // an item with no contents
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    setFlag(QGraphicsItem::ItemClipsChildrenToShape);

    m_effect = new MaskEffect();
    setGraphicsEffect(m_effect);
}

MDeclarativeMaskedItem::~MDeclarativeMaskedItem()
{

}

/*!
  \qmlproperty Component MaskedItem::mask

  The mask item defines the image of the masking effect applied over the
  children. Only the root item of the mask component will be painted to
  create the mask.
*/

QDeclarativeComponent *MDeclarativeMaskedItem::mask() const
{
    return m_maskComponent;
}

void MDeclarativeMaskedItem::setMask(QDeclarativeComponent *component)
{
    if (m_maskComponent == component)
        return;

    QDeclarativeItem *mask = 0;

    if (component) {
        QObject *object = component->create(component->creationContext());
        mask = qobject_cast<QDeclarativeItem *>(object);

        if (!mask)
            qWarning("MaskedItem: Unable to create mask element.");
        else if (!mask->childItems().isEmpty())
            qWarning("MaskedItem: Mask element has children. Due to current limitation, they won't be painted.");
    }

    m_effect->setMask(mask);
    m_maskComponent = component;

    emit maskChanged();
}

#include "mdeclarativemaskeditem.moc"
