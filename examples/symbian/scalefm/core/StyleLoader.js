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

var sourcePath
var prevPath = ""
var object = null

function roundNearestFive(value) {
    return Math.round(value / 5.0) * 5
}

function path(componentName, source) {
    var id = componentName.lastIndexOf("/")
    if (id < 0)
        id = componentName.lastIndexOf("\\")
    var src = ""
    if (id >= 0)
        src += componentName.substr(0, id + 1)
    src += source
    src += componentName.substr(id + 1)
    return src
}

function displayCategory() {
    switch (screen.displayCategory) {
        case Screen.Small: return "Small"
        case Screen.Normal: return "Normal"
        case Screen.Large: return "Large"
        case Screen.ExtraLarge:
        default:  return "ExtraLarge"
    }
}

function displayDensity() {
    switch (screen.density) {
        case Screen.Low: return "Low"
        case Screen.Medium: return "Medium"
        case Screen.High: return "High"
        case Screen.ExtraHigh:
        default:  return "ExtraHigh"
    }
}

function loadStyle(componentName, container, forceFallback) {
    var source = ""
    var fallback = source

    if (!forceFallback)
        source = Math.max(screen.width, screen.height) + "_" + Math.min(screen.width, screen.height) + "/" + roundNearestFive(screen.dpi) + "/"

    sourcePath = path(componentName, source)
    if (sourcePath != prevPath) {
        console.log("Loading: " + sourcePath)
        var component = Qt.createComponent(sourcePath)
        if (component.status != Component.Ready) {
            console.log("File not found, falling back to categories")
            var category = displayCategory() + "/" + displayDensity() + "/"
            sourcePath = path(componentName, category)
            if (sourcePath != prevPath) {
                console.log("Loading: " + sourcePath)
                component = Qt.createComponent(sourcePath)
                if (component.status != Component.Ready) {
                    console.log("File not found, falling back to default")
                    sourcePath = path(componentName, fallback)
                    if (sourcePath != prevPath) {
                        console.log("Loading: " + sourcePath)
                        component = Qt.createComponent(sourcePath)
                        if (component.status != Component.Ready)
                            console.log("Default file not found!")
                    }
                }
            }
        }
        if (component.status == Component.Ready) {
            object = component.createObject(container)
            prevPath = sourcePath
        }
    }
    return object
}
