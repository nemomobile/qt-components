/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
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

#ifndef MXLIKEBUTTONGROUP_H
#define MXLIKEBUTTONGROUP_H

#include <QtDeclarative/QDeclarativeItem>

class Q_DECLARATIVE_EXPORT MxLikeButtonGroup : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY(QDeclarativeItem *checkedItem READ checkedItem NOTIFY checkedItemChanged)
    Q_PROPERTY(bool allowNoChecked READ allowNoChecked WRITE setAllowNoChecked) // ### NOTIFY

public:
    MxLikeButtonGroup(QDeclarativeItem *parent = 0);
    virtual ~MxLikeButtonGroup();

    QDeclarativeItem *checkedItem() {
        return m_checkedItem;
    }

    bool allowNoChecked() const {
        return m_allowNoChecked;
    }

    void setAllowNoChecked(bool allow) {
        m_allowNoChecked = allow;
    }

    virtual void componentComplete();

public Q_SLOTS:
    void refresh();

signals:
    void checkedItemChanged(QDeclarativeItem *item);

private Q_SLOTS:
    void onItemChecked();
    void onItemDestroyed();

private:
    void connectChildrenItems(QDeclarativeItem *item);

    QDeclarativeItem *m_checkedItem;
    QList<QDeclarativeItem *> m_items;
    bool m_allowNoChecked;
};

#endif
