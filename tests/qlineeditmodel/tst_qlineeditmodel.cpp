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

#include <QtTest/QtTest>
#include "qlineeditmodel.h"

class tst_QLineEditModel : public QObject
{
    Q_OBJECT

public:
    tst_QLineEditModel();
    virtual ~tst_QLineEditModel();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void getSetCheck();

protected:
    QLineEditModel *model;
};

tst_QLineEditModel::tst_QLineEditModel()
{
}

tst_QLineEditModel::~tst_QLineEditModel()
{
}

void tst_QLineEditModel::initTestCase()
{
}

void tst_QLineEditModel::cleanupTestCase()
{
}

void tst_QLineEditModel::init()
{
    model = new QLineEditModel();
}

void tst_QLineEditModel::cleanup()
{
    delete model;
}

void tst_QLineEditModel::getSetCheck()
{

}

QTEST_MAIN(tst_QLineEditModel)
#include "tst_qlineeditmodel.moc"
