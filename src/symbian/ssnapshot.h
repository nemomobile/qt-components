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

#ifndef MSNAPSHOT_H
#define MSNAPSHOT_H

#include <qdeclarativeitem.h>
#include <qglobal.h>
#include <qpixmap.h>

class Snapshot : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(int snapshotWidth READ snapshotWidth WRITE setSnapshotWidth NOTIFY snapshotWidthChanged)
    Q_PROPERTY(int snapshotHeight READ snapshotHeight WRITE setSnapshotHeight NOTIFY snapshotHeightChanged)

public:
    Snapshot(QDeclarativeItem *parent = 0);
    virtual ~Snapshot();

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

    int snapshotWidth() const;
    int snapshotHeight() const;

    void setSnapshotWidth(int width);
    void setSnapshotHeight(int height);

public Q_SLOTS:
    void take();
    void free();

Q_SIGNALS:
    void snapshotWidthChanged();
    void snapshotHeightChanged();

private:
    Q_DISABLE_COPY(Snapshot)

    QPixmap m_snapshot;
    int m_width;
    int m_height;
};

QML_DECLARE_TYPE(Snapshot)
#endif
