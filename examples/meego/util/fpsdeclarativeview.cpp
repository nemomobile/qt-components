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

#include <QDeclarativeView>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>
#include <QTime>
#include <QWidget>
#include <QFont>
#include <QRect>
#include <QFontMetrics>
#include <QUrl>

#include "fpsdeclarativeview.h"

FPSDeclarativeView::FPSDeclarativeView(QWidget *parent) :
        QDeclarativeView(parent),
        m_frames(0),
        m_fps(0),
        m_showFPS(false),
        m_fpsMeasured(false)
{
    initFPSRect();
}

FPSDeclarativeView::FPSDeclarativeView(const QUrl &source, QWidget *parent) :
        QDeclarativeView(source, parent),
        m_frames(0),
        m_fps(0),
        m_showFPS(false),
        m_fpsMeasured(false)
{
    initFPSRect();
}

FPSDeclarativeView::~FPSDeclarativeView()
{
}

void FPSDeclarativeView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_F) {
        m_showFPS = !m_showFPS;
        scene()->update();
    }
    QDeclarativeView::keyPressEvent(event);
}

void FPSDeclarativeView::resizeEvent(QResizeEvent *event)
{
    QDeclarativeView::resizeEvent(event);
    initFPSRect();
}


void FPSDeclarativeView::drawForeground(QPainter *p, const QRectF &rect)
{
    QDeclarativeView::drawForeground(p, rect);
    if (m_showFPS) {
        QFont npf("Nokia Pure");
        npf.setPointSize(18);
        p->setPen(Qt::red);
        p->setFont(npf);
        p->drawText(m_fpsRect, Qt::AlignRight, m_fpsMeasured ? "FPS: " + QString::number(m_fps) : "FPS: n/a");
    }
}

void FPSDeclarativeView::initFPSRect()
{
    QFont f("Nokia Pure");
    f.setPointSize(24);
    QFontMetrics metrics(f);
    int fpsRectHeight = metrics.height();
    int fpsRectWidth  = metrics.width("FPS: 0000");
    m_fpsRect = QRect(width()-fpsRectWidth-100, height()-fpsRectHeight-10, fpsRectWidth, fpsRectHeight);

}

void FPSDeclarativeView::paintEvent(QPaintEvent *event)
{

    if (m_showFPS) {
        if (m_frames == 0) {
            m_timer.start();
        } else if (m_timer.elapsed() > 3000) {
            m_fps = 1000 * ((qreal) m_frames / m_timer.elapsed());
            m_timer.start();
            m_frames = 0;
            m_fpsMeasured = true;
        }

        m_frames++;
    }

    QDeclarativeView::paintEvent(event);

    if (m_showFPS)
        scene()->update();

}
