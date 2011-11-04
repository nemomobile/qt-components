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

#include "utils.h"

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QClipboard>


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

void Settings::clearClipboard() const {
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
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

void Settings::setOrientationMethod(int orientationMethod)
{
    QFileInfo info(QApplication::applicationFilePath());
    QSettings settings("Nokia", info.baseName());
    settings.setValue("orientationMethod", orientationMethod);
}

int Settings::orientationMethod() const
{
    QFileInfo info(QApplication::applicationFilePath());
    QSettings settings("Nokia", info.baseName());
    return settings.value("orientationMethod").toInt();
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

void Settings::setTime(int hours, int minutes)
{
#ifdef Q_OS_SYMBIAN
    TTime currentTime;
    currentTime.HomeTime();
    TDateTime dateTime = currentTime.DateTime();
    dateTime.SetHour((TInt)hours);
    dateTime.SetMinute((TInt)minutes);
    TTime newTime(dateTime);
    User::SetHomeTime(newTime);
#else
    qDebug() << "Setting time " << hours << ":" << minutes << " (does not work in desktop)";
#endif
}

LayoutDirectionSetter::LayoutDirectionSetter(QObject *parent) : QObject(parent)
{
}

void LayoutDirectionSetter::setLayoutDirection(int direction)
{
    if (direction == Qt::LeftToRight || direction == Qt::RightToLeft || direction == Qt::LayoutDirectionAuto)
        static_cast<QApplication *>(QApplication::instance())->setLayoutDirection((Qt::LayoutDirection)direction);
}
