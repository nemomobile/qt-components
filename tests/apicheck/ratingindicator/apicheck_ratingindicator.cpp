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

#include <QtTest/QtTest>
#include "apicheck_ratingindicator.h"


void ApiCheckRatingIndicator::initTestCase()
{
    init("RatingIndicator");
}

void ApiCheckRatingIndicator::maximumValue()
{
    validateProperty("maximumValue", static_cast<QVariant::Type>(QMetaType::QReal), 0.0);
}

void ApiCheckRatingIndicator::ratingValue()
{
    validateProperty("ratingValue", static_cast<QVariant::Type>(QMetaType::QReal), 0.0);
}

void ApiCheckRatingIndicator::count()
{
    validateProperty("count", QVariant::Int);
}

void ApiCheckRatingIndicator::inverted()
{
    validateProperty("inverted", QVariant::Bool);
}
