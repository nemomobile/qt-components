/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Trolltech Labs.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QDECLARATIVESLIDER_H
#define QDECLARATIVESLIDER_H

#include <QtDeclarative/qdeclarativeitem.h>

class QDeclarativeSliderPrivate;

class QDeclarativeSlider : public QDeclarativeItem
{
    Q_OBJECT;

    Q_PROPERTY(int minimumValue READ minimum WRITE setMinimum NOTIFY rangeChanged)
    Q_PROPERTY(int maximumValue READ maximum WRITE setMaximum NOTIFY rangeChanged)
    Q_PROPERTY(int singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(int pageStep READ pageStep WRITE setPageStep)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(bool tracking READ hasTracking WRITE setTracking)

public:
    QDeclarativeSlider(QDeclarativeItem *parent = 0);
    virtual ~QDeclarativeSlider();

    void setMinimum(int);
    int minimum() const;

    void setMaximum(int);
    int maximum() const;

    void setRange(int min, int max);

    void setSingleStep(int);
    int singleStep() const;

    void setPageStep(int);
    int pageStep() const;

    void setValue(int);
    int value() const;

    void setTracking(bool enable);
    bool hasTracking() const;

Q_SIGNALS:
    void valueChanged(int value);
    void rangeChanged(int min, int max);

protected:
    QDeclarativeSlider(QDeclarativeSliderPrivate &dd, QDeclarativeItem *parent = 0);

private:
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeSlider);
    Q_DISABLE_COPY(QDeclarativeSlider);
};

#endif

