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

/// Helper code for handling Actions, used by Window, assumes
/// 'toolbarContents' identifier that will be the container of the
/// action items.

var appActions = [];
var pageActions = [];

function removeActions(actions) {
    var i;
    var c;

    for (i = 0; i < actions.length; i++) {
        c = actions[i];
        c.item.parent = null;
        c.__hidden = true;
        c.item.visible = false;
        c.interactiveChanged.disconnect(c, onActionInteractiveChanged);
    }
}

function addAction(a, actions) {
    actions.push(a);
    a.item.parent = toolbarContents;
    a.__hidden = false;
    a.item.visible = a.interactive;

    a.interactiveChanged.connect(a, onActionInteractiveChanged);
}

function setActionsForApp(actions) {
    removeActions(appActions);
    appActions = [];

    var i;
    for (i = 0; i < actions.length; i++) {
        addAction(actions[i], appActions);
    }

    // ### To enforce the correct order (_app_ actions first) we remove
    // and add again the _page_ actions.
    setActionsForPage(pageActions, toolbarContents);

    updateInteractiveActions();
}

function setActionsForPage(actions) {
    removeActions(pageActions);
    pageActions = [];

    var i;
    for (i = 0; i < actions.length; i++) {
        addAction(actions[i], pageActions);
    }

    updateInteractiveActions();
}

function onActionInteractiveChanged() {
    var action = this;
    updateInteractiveActions();
    action.item.visible = action.interactive && !action.__hidden;
}

function updateInteractiveActions() {
    var i;

    for (i = 0; i < appActions.length; i++) {
        if (appActions[i].interactive) {
            toolbarContents.hasInteractiveActions = true;
            return;
        }
    }

    for (i = 0; i < pageActions.length; i++) {
        if (pageActions[i].interactive) {
            toolbarContents.hasInteractiveActions = true;
            return;
        }
    }

    toolbarContents.hasInteractiveActions = false;
}
