/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#include "sdeclarativeindicator.h"
#include "sdeclarativeindicatordata.h"

#include <aknconsts.h> // KAvkonBitmapFile
#include <AknsUtils.h> // AknsUtils
#include <QPainter>

// #define Q_DEBUG_INDICATOR

#ifdef Q_DEBUG_INDICATOR
#include <QDebug>
#endif // Q_DEBUG_INDICATOR

const int INDICATOR_NOT_SHOWN = -1;

class SDeclarativeIndicatorPrivate
{
public:
    SDeclarativeIndicatorPrivate(CSDeclarativeIndicatorData *data, SDeclarativeIndicator *qq);
    ~SDeclarativeIndicatorPrivate();

    void ensureIndicatorBitmaps();
    void deleteBitmaps();
    void setSvgIconSize(CFbsBitmap *bitmap) const;
    QPixmap currentFramePixmap();

    CSDeclarativeIndicatorData *data;
    QList<CFbsBitmap *> indicatorBitmaps;
    QColor color;

private:
    QHash<int, QPixmap> pixmaps;
    SDeclarativeIndicator *q_ptr;
};

SDeclarativeIndicatorPrivate::SDeclarativeIndicatorPrivate(
    CSDeclarativeIndicatorData *data, SDeclarativeIndicator *qq)
    : data(data), q_ptr(qq)
{
    ensureIndicatorBitmaps();
}

SDeclarativeIndicatorPrivate::~SDeclarativeIndicatorPrivate()
{
    deleteBitmaps();
}

void SDeclarativeIndicatorPrivate::ensureIndicatorBitmaps()
{
    if (!indicatorBitmaps.count()) {
        TInt bitmapId = 0;
        TInt maskId = 0;
        CFbsBitmap *bitmap = 0;
        CFbsBitmap *mask = 0;
        RArray<TInt> bitmapIndexes;
        data->GetIndicatorBitmapIndexes( bitmapIndexes );
        const TInt count = bitmapIndexes.Count();

        for (TInt ii = 0; ii < count; ii += 2) {
            bitmapId = bitmapIndexes[ii];
            maskId = bitmapIndexes[ii + 1];

            if (bitmapId && maskId) {
                const TAknsItemID dummyId = {0, 0};
                const TAknsItemID dummyColorGroup = {0, 0};
                const TInt dummyColorIndex = 0;

                const TDesC &fileName = data->BitmapFile().Length() ? data->BitmapFile() : AknIconUtils::AvkonIconFileName();
                const TRgb symbianColor(color.red(), color.green(), color.blue(), color.alpha());

                QT_TRAP_THROWING({
                    if (data->MultiColorMode())
                        AknIconUtils::CreateIconL(
                            bitmap,
                            mask,
                            fileName,
                            bitmapId,
                            maskId);
                    else
                        AknsUtils::CreateColorIconL(
                            AknsUtils::SkinInstance(),
                            dummyId,
                            dummyColorGroup,
                            dummyColorIndex,
                            bitmap,
                            mask,
                            fileName,
                            bitmapId,
                            maskId,
                            symbianColor);
                    })

                indicatorBitmaps.append(bitmap);
                indicatorBitmaps.append(mask);

                if (bitmap)
                    setSvgIconSize(bitmap);

                if (mask)
                    setSvgIconSize(mask);
            }
        }
        bitmapIndexes.Close();
    }
}

void SDeclarativeIndicatorPrivate::deleteBitmaps()
{
    pixmaps.clear();
    while (indicatorBitmaps.count())
        delete indicatorBitmaps.takeLast();
}

void SDeclarativeIndicatorPrivate::setSvgIconSize(CFbsBitmap *bitmap) const
{
    const TInt KFirstStatusPaneIndicator = EAknIndicatorIrActive;
    const TInt KLastStatusPaneIndicator = EAknNaviPaneEditorIndicatorT9 - 1;

    if ( data->Uid() >= KFirstStatusPaneIndicator &&
         data->Uid() <= KLastStatusPaneIndicator &&
         data->Priority() != INDICATOR_NOT_SHOWN) {

        const TScaleMode aspectRatio = EAspectRatioPreservedAndUnusedSpaceRemoved;
        const TSize iconSize = TSize(qRound(q_ptr->width()), qRound(q_ptr->height()));
        AknIconUtils::SetSize(bitmap, iconSize, aspectRatio);
    }
}

QPixmap SDeclarativeIndicatorPrivate::currentFramePixmap()
{
    const int bitmapIndex = data->CurrentFrame() * 2;
    const int maskIndex = bitmapIndex + 1;

    if (!pixmaps.contains(bitmapIndex)) {
        if (maskIndex < indicatorBitmaps.count()) {
            QPixmap iconPixamp = QPixmap::fromSymbianCFbsBitmap(indicatorBitmaps.at(bitmapIndex));
            QPixmap maskPixamp = QPixmap::fromSymbianCFbsBitmap(indicatorBitmaps.at(maskIndex));
            iconPixamp.setAlphaChannel(maskPixamp);
            pixmaps.insert(bitmapIndex, iconPixamp);
        } else if (bitmapIndex < indicatorBitmaps.count()) {
            CFbsBitmap* mask = indicatorBitmaps.at(bitmapIndex);
            QPixmap maskPixamp = QPixmap::fromSymbianCFbsBitmap(mask);
            pixmaps.insert(bitmapIndex, maskPixamp);
        } else {
            // not found. Do not fill the cache with dummy items.
#ifdef Q_DEBUG_INDICATOR
            qDebug() << "SDeclarativeIndicatorPrivate::currentFramePixmap() bitmap not found";
#endif
            return QPixmap();
        }
    }
    return pixmaps.value(bitmapIndex);
}


SDeclarativeIndicator::SDeclarativeIndicator(CSDeclarativeIndicatorData *data, QDeclarativeItem *parent)
    : QDeclarativeItem(parent), d_ptr(new SDeclarativeIndicatorPrivate(data, this))
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
#ifdef Q_DEBUG_INDICATOR
    qDebug() << "SDeclarativeIndicator created. Uid: " << data->Uid() << " priority: " << data->Priority();
#endif // Q_DEBUG_INDICATOR
}

SDeclarativeIndicator::~SDeclarativeIndicator()
{
#ifdef Q_DEBUG_INDICATOR
    Q_D(SDeclarativeIndicator);
    qDebug() << "SDeclarativeIndicator deleted. Uid: " << d->data->Uid();
#endif // Q_DEBUG_INDICATOR
}

int SDeclarativeIndicator::uid() const
{
    Q_D(const SDeclarativeIndicator);
    return d->data->Uid();
}

int SDeclarativeIndicator::priority() const
{
    Q_D(const SDeclarativeIndicator);
    return d->data->Priority();
}

int SDeclarativeIndicator::state() const
{
    Q_D(const SDeclarativeIndicator);
    return d->data->State();
}

bool SDeclarativeIndicator::setState(int state)
{
    Q_D(SDeclarativeIndicator);

#ifdef Q_DEBUG_INDICATOR
    qDebug() << "SDeclarativeIndicator::setState from " << d->data->State() << " to " << state;
#endif // Q_DEBUG_INDICATOR

    bool changed = false;
    if (d->data->State() != state) {
        changed = true;
        d->data->SetState(state);
        if (priority() == INDICATOR_NOT_SHOWN ) {
            setVisible(false);
        } else {
            if (d->data->State() == EAknIndicatorStateOn) {
                d->data->SetCurrentFrame(d->data->FrameCount() - 1);
                setVisible(true);
            } else if (d->data->State() == EAknIndicatorStateOff) {
                setVisible(false);
                d->deleteBitmaps();
            } else if (d->data->State() == EAknIndicatorStateAnimate) {
                setVisible(true);
            }
        }

        update();
    }
    return changed;
}

QColor SDeclarativeIndicator::color() const
{
    Q_D(const SDeclarativeIndicator);
    return d->color;
}

void SDeclarativeIndicator::setColor(const QColor &color)
{
    Q_D(SDeclarativeIndicator);
    if (d->color != color) {
        d->color = color;
        d->deleteBitmaps();
        emit colorChanged();
    }
}

void SDeclarativeIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_D(SDeclarativeIndicator);

#ifdef Q_DEBUG_INDICATOR
    painter->drawRect(-x(), -y(), parentItem()->width(), parentItem()->height() );
#endif // Q_DEBUG_INDICATOR

    d->ensureIndicatorBitmaps();

    if (d->indicatorBitmaps.count() <= 0 || !d->indicatorBitmaps.at(0)) {
#ifdef Q_DEBUG_INDICATOR
        qDebug() << "SDeclarativeIndicator::paint() error: no bitmaps loaded";
#endif // Q_DEBUG_INDICATOR
        return;
    }

    if (d->data->State() != EAknIndicatorStateOff) {
        // GPRS indicator is actually never drawn here, it is drawn
        // by the system to the signal pane.
        if (d->data->Uid() == EAknNaviPaneEditorIndicatorGprs) {
            return;
        }

        // center the current frame pixmap
        QPixmap pixmap = d->currentFramePixmap();
        painter->drawPixmap(QPoint((width() - pixmap.width()) / 2, (height() - pixmap.height()) / 2), pixmap);

#ifdef Q_DEBUG_INDICATOR
        painter->drawRect(0, 0, width() , height());
#endif // Q_DEBUG_INDICATOR
    }
}

void SDeclarativeIndicator::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_D(SDeclarativeIndicator);
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);

    if (oldGeometry.size() != newGeometry.size())
        d->deleteBitmaps();
}


