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

#ifndef SDECLARATIVESTYLE_H
#define SDECLARATIVESTYLE_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qcolor.h>

class SDeclarativeStylePrivate;
class SStyleEngine;

class SDeclarativeStyle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fontFamilyRegular READ fontFamilyRegular CONSTANT FINAL)

    Q_PROPERTY(int fontSizeLarge READ fontSizeLarge CONSTANT FINAL)
    Q_PROPERTY(int fontSizeMedium READ fontSizeMedium CONSTANT FINAL)
    Q_PROPERTY(int fontSizeSmall READ fontSizeSmall CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeLarge READ graphicSizeLarge CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeMedium READ graphicSizeMedium CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeSmall READ graphicSizeSmall CONSTANT FINAL)
    Q_PROPERTY(int graphicSizeTiny READ graphicSizeTiny CONSTANT FINAL)
    Q_PROPERTY(int paddingLarge READ paddingLarge CONSTANT FINAL)
    Q_PROPERTY(int paddingMedium READ paddingMedium CONSTANT FINAL)
    Q_PROPERTY(int paddingSmall READ paddingSmall CONSTANT FINAL)
    Q_PROPERTY(int borderSizeMedium READ borderSizeMedium CONSTANT FINAL)

    Q_PROPERTY(QColor colorNormalLight READ colorNormalLight CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalMid READ colorNormalMid CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalDark READ colorNormalDark CONSTANT FINAL)
    Q_PROPERTY(QColor colorNormalLink READ colorNormalLink CONSTANT FINAL)
    Q_PROPERTY(QColor colorPressed READ colorPressed CONSTANT FINAL)
    Q_PROPERTY(QColor colorChecked READ colorChecked CONSTANT FINAL)
    Q_PROPERTY(QColor colorHighlighted READ colorHighlighted CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledLight READ colorDisabledLight CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledMid READ colorDisabledMid CONSTANT FINAL)
    Q_PROPERTY(QColor colorDisabledDark READ colorDisabledDark CONSTANT FINAL)
    Q_PROPERTY(QColor colorTextSelection READ colorTextSelection CONSTANT FINAL)

public:

    explicit SDeclarativeStyle(SStyleEngine *engine, QObject *parent = 0);
    ~SDeclarativeStyle();

    QString fontFamilyRegular() const;

    int fontSizeLarge() const;
    int fontSizeMedium() const;
    int fontSizeSmall() const;
    int graphicSizeLarge() const;
    int graphicSizeMedium() const;
    int graphicSizeSmall() const;
    int graphicSizeTiny() const;
    int paddingLarge() const;
    int paddingMedium() const;
    int paddingSmall() const;
    int borderSizeMedium() const;

    QColor colorNormalLight() const;
    QColor colorNormalMid() const;
    QColor colorNormalDark() const;
    QColor colorNormalLink() const;
    QColor colorPressed() const;
    QColor colorChecked() const;
    QColor colorHighlighted() const;
    QColor colorDisabledLight() const;
    QColor colorDisabledMid() const;
    QColor colorDisabledDark() const;
    QColor colorTextSelection() const;

Q_SIGNALS:
    void fontParametersChanged();
    void layoutParametersChanged();
    void colorParametersChanged();

protected:
    QScopedPointer<SDeclarativeStylePrivate> d_ptr;

private:
    Q_DISABLE_COPY(SDeclarativeStyle)
    Q_DECLARE_PRIVATE(SDeclarativeStyle)
};

#endif // SDECLARATIVESTYLE_H
