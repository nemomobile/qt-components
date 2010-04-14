/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
