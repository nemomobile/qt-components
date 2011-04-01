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

var allContainers = []
var allContents = []

function hasContainer(content) {
    for (var i = 0; i < allContainers.length; ++i) {
        if (allContainers[i].children[0] == content)
            return true
    }
    return false
}

function addContent(content) {
    for (var i = 0; i < allContents.length; ++i) {
        if (allContents[i] == content)
            return false
    }
    allContents.push(content)
    return true
}

function ensureContainers() {
    var somethingChanged = false

    // check if we need to create a container
    for (var i = 0; i < root.privateContents.length; ++i) {
        var content = root.privateContents[i]
        addContent(content)
        if (!hasContainer(content)) {
            var newContainer = tabContainerComponent.createObject(containerHost)
            content.parent = newContainer
            allContainers.push(newContainer)
            somethingChanged = true
        }
    }
    return somethingChanged
}

function addTab(content) {
    if (addContent(content)) {
        var newContainer = tabContainerComponent.createObject(containerHost)
        content.parent = newContainer
        allContainers.push(newContainer)
    }
}

function removeTab(content) {
    var foundIndex = -1
    for (var i = 0; i < allContents.length && foundIndex == -1; ++i) {
        if (allContents[i] == content)
            foundIndex = i
    }

    if (foundIndex != -1)
        allContents.splice(foundIndex, 1)

    if (hasContainer(content))
        content.parent = null // this causes deletion of container
}

function removeContainer(container) {
    var foundIndex = -1
    for (var i = 0; i < allContainers.length && foundIndex == -1; ++i) {
        if (allContainers[i] == container)
            foundIndex = i
    }

    if (foundIndex != -1) {
        var deletedContainer = allContainers[foundIndex]
        if (deletedContainer.children.length > 0)
            removeTab(deletedContainer.children[0])
        allContainers.splice(foundIndex, 1)
        deletedContainer.destroy()
    }
}

