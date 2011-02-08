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

#if defined(Q_COMPONENTS_SYMBIAN3) && !defined(Q_OS_SYMBIAN)
#include "settingswindow.h"
#endif
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QTime>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QLibraryInfo>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QSettings>
#include <QList>

#ifdef Q_OS_SYMBIAN
static const QStringList QML_PATHS = QStringList() << "e:/qmlc" << "c:/data/qmlc";
#else
static const QStringList QML_PATHS = QStringList() << "qmlc/";
#endif

class FileAccess : public QObject
{
    Q_OBJECT

public:
    explicit FileAccess(QObject *parent = 0) : QObject(parent) {}
    ~FileAccess() {}

    Q_INVOKABLE QStringList qmlFileNames(const QString &path) {
        QStringList filePaths;
        QList<QFileInfo> entries = QDir(path).entryInfoList(QStringList("*.qml"), QDir::Files);
        foreach (const QFileInfo &info, entries)
            filePaths += info.fileName();
        filePaths.replaceInStrings(".qml", "");
        return filePaths;
    }

    Q_INVOKABLE QStringList qmlFilePaths(const QString &path) {
        QStringList filePaths;
        QList<QFileInfo> entries = QDir(path).entryInfoList(QStringList("*.qml"), QDir::Files);
        foreach (const QFileInfo &info, entries)
            filePaths += info.absoluteFilePath();
        return filePaths;
    }

    Q_INVOKABLE QStringList qmlPaths() {
        QStringList paths;
        foreach (const QString &path, QML_PATHS) {
            QDir dir;
            if (!dir.exists(path) && !dir.mkdir(path))
                qWarning() << "Creation of " << path << " failed";
            else
                paths += path;
        }
        return paths;
    }
};

class Settings : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject *parent = 0) : QObject(parent) {}
    ~Settings() {}

    Q_INVOKABLE void setOrientation(int o) {
        QSettings settings("componenttest");
        settings.setValue("orientation", o);
    }

    Q_INVOKABLE int orientation() {
        QSettings settings("componenttest");
        return settings.value("orientation").toInt();
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qmlRegisterType<FileAccess>("FileAccess", 1, 0, "FileAccess");
    qmlRegisterType<Settings>("Settings", 1, 0, "Settings");

    QDeclarativeView view;
    view.engine()->addImportPath(Q_COMPONENTS_BUILD_TREE"/imports");

#ifdef Q_OS_SYMBIAN
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.showMaximized();
#else
    QDir::setCurrent(app.applicationDirPath());
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();
#endif

#if defined(Q_COMPONENTS_SYMBIAN3) && !defined(Q_OS_SYMBIAN)
    SettingsWindow settingsWindow(&view);
#endif
    return app.exec();
}

#include "componenttest.moc"
