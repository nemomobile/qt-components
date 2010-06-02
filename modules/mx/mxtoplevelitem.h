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
#ifndef MXTOPLEVELITEM_H
#define MXTOPLEVELITEM_H

#include <QtDeclarative/qdeclarativeitem.h>

class MxTopLevelItemPrivate;
class MxTopLevelItem : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(bool keepInside READ keepInside WRITE setKeepInside NOTIFY keepInsideChanged);

public:
    MxTopLevelItem(QDeclarativeItem *parent = 0);
    virtual ~MxTopLevelItem();

    bool keepInside() const;
    void setKeepInside(bool keepInside);

Q_SIGNALS:
    void keepInsideChanged(bool keepInside);

protected:
    MxTopLevelItem(MxTopLevelItemPrivate &dd, QDeclarativeItem *parent = 0);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QScopedPointer<MxTopLevelItemPrivate> d_ptr;

private:
    Q_DISABLE_COPY(MxTopLevelItem);
    Q_DECLARE_PRIVATE(MxTopLevelItem);
};

QML_DECLARE_TYPE(MxTopLevelItem)

#endif // MXTOPLEVELITEM_H
