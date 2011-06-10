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

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include <stdio.h>
#include <stdlib.h>

void fail(QString const& message)
{
    printf("CHECKTEST FAIL: %s\n", qPrintable(message));
    exit(0);
}

void pass(QString const& message)
{
    printf("CHECKTEST PASS: %s\n", qPrintable(message));
    exit(0);
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();
    args.removeFirst(); // ourself

    QString args_quoted = QString("'%1'").arg(args.join("','"));

#ifdef Q_WS_QWS
    {
        // for QWS we expect tests to be run as the QWS server
        QString qws = args.takeLast();
        if (qws != "-qws") {
            fail(QString("Expected test to be run with `-qws', but it wasn't; args: %1").arg(args_quoted));
        }
    }
#endif

    if (args.count() != 1) {
        fail(QString("These arguments are not what I expected: %1").arg(args_quoted));
    }

    QString test = args.at(0);

    QFileInfo testfile(test);
    if (!testfile.exists()) {
        fail(QString("File %1 does not exist (my working directory is: %2, my args are: %3)")
            .arg(test)
            .arg(QDir::currentPath())
            .arg(args_quoted)
        );
    }

    pass(args_quoted);
}

