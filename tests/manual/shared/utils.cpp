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

#include "utils.h"

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>


#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
#include <avkon.hrh>
#include <aknsmallindicator.h>
#else
// RnD: hosts fake indicator rectangles.
QHash<int, QDeclarativeItem *> indicatorItems;
#endif // Q_OS_SYMBIAN & HAVE_SYMBIAN_INTERNAL

#ifdef Q_OS_SYMBIAN
static const QStringList QML_PATHS = QStringList() << "e:/qmlc" << "c:/data/qmlc";
#else
static const QStringList QML_PATHS = QStringList() << "qmlc/" << "../../../doc/src/snippets/symbian";
#endif

FileAccess::FileAccess(QObject *parent) : QObject(parent)
{
}

QStringList FileAccess::qmlFileNames(const QString &path) const
{
    QStringList filePaths;
    QList<QFileInfo> entries = QDir(path).entryInfoList(QStringList("*.qml"), QDir::Files);
    foreach (const QFileInfo &info, entries)
        filePaths += info.fileName();
    filePaths.replaceInStrings(".qml", "");
    return filePaths;
}

QStringList FileAccess::qmlFilePaths(const QString &path) const
{
    QStringList filePaths;
    QList<QFileInfo> entries = QDir(path).entryInfoList(QStringList("*.qml"), QDir::Files);
    foreach (const QFileInfo &info, entries)
        filePaths += info.absoluteFilePath();
    return filePaths;
}

QStringList FileAccess::qmlPaths() const
{
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

Settings::Settings(QObject *parent) : QObject(parent)
{
}

Settings::~Settings()
{
#if !defined(Q_OS_SYMBIAN) || !defined(HAVE_SYMBIAN_INTERNAL)
    indicatorItems.clear();
#endif
}

void Settings::setOrientation(int orientation) {
    QFileInfo info(QApplication::applicationFilePath());
    QSettings settings("Nokia", info.baseName());
    settings.setValue("orientation", orientation);
}

int Settings::orientation() const {
    QFileInfo info(QApplication::applicationFilePath());
    QSettings settings("Nokia", info.baseName());
    return settings.value("orientation").toInt();
}

void Settings::setIndicatorState(int indicatorId, bool on) const
{
#if defined(Q_OS_SYMBIAN) && defined(HAVE_SYMBIAN_INTERNAL)
    QT_TRAP_THROWING({
        CAknSmallIndicator* smallIndicator = CAknSmallIndicator::NewLC( TUid::Uid( indicatorId ) );
        smallIndicator->SetIndicatorStateL( on ? EAknIndicatorStateOn : EAknIndicatorStateOff );
        CleanupStack::PopAndDestroy( smallIndicator );
    });
#else
    // Rnd: add rectanges to represent real indicators

    // find indicator container via root:
    const QObject *root = this;
    while (root->parent())
        root = root->parent();

    QDeclarativeItem *indicatorContainer = root->findChild<QDeclarativeItem *>(QLatin1String("indicators"));

    if (indicatorContainer) {
        QDeclarativeItem *item = 0;
        if (!indicatorItems.contains(indicatorId)) {
            QDeclarativeEngine *engine = QDeclarativeEngine::contextForObject(indicatorContainer)->engine();
            QDeclarativeComponent component(engine);
            component.setData("import Qt 4.7\nRectangle { border.color: parent ? parent.indicatorColor : \"gray\" }", QUrl());
            item = qobject_cast<QDeclarativeItem *>(component.create(QDeclarativeEngine::contextForObject(indicatorContainer)));
            if (component.isError()) {
                qDebug() << component.errorString();
                return;
            }

            // use "random" color
            QColor color(qptrdiff(item)%255,qptrdiff(item)%255,qptrdiff(item)%255);
            item->setProperty("color", QVariant::fromValue(color));
            item->setParentItem(indicatorContainer);
            indicatorItems.insert(indicatorId, item);
        }

        item = indicatorItems.value(indicatorId);
        item->setVisible(on);
        QMetaObject::invokeMethod(indicatorContainer, "layoutChildren");
    }

#endif // Q_OS_SYMBIAN & HAVE_SYMBIAN_INTERNAL
}
