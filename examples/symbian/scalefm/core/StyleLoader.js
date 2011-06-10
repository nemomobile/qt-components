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
