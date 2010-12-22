/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SDECLARATIVEFRAME_H
#define SDECLARATIVEFRAME_H

#include <QtCore/qscopedpointer.h>
#include <QtDeclarative/qdeclarativeitem.h>

class SDeclarativeFramePrivate;

class SDeclarativeFrame : public QDeclarativeItem
{
    Q_OBJECT
    Q_ENUMS(FrameType)
    Q_PROPERTY(QString frameName READ frameName WRITE setFrameName NOTIFY frameNameChanged)
    Q_PROPERTY(FrameType frameType READ frameType WRITE setFrameType NOTIFY frameTypeChanged)

public:
    SDeclarativeFrame(QDeclarativeItem *parent = 0);
    virtual ~SDeclarativeFrame();

    QString frameName() const;
    void setFrameName(const QString &frameName);

    enum FrameType {
        Undefined,
        OnePiece,
        ThreePiecesHorizontal,
        ThreePiecesVertical,
        NinePieces
    };

    FrameType frameType() const;
    void setFrameType(FrameType type);

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

Q_SIGNALS:
    void frameNameChanged(const QString &frameName);
    void frameTypeChanged(SDeclarativeFrame::FrameType frameType);

protected:
    QScopedPointer<SDeclarativeFramePrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeFrame)
    Q_DECLARE_PRIVATE(SDeclarativeFrame)
};

QML_DECLARE_TYPE(SDeclarativeFrame)

#endif // SDECLARATIVEFRAME_H
