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

// The PageStack item defines a container for pages and a stack-based
// navigation model. Pages can be defined as QML items or components.

import Qt 4.7
import com.meego.themebridge 1.0
import "PageStack.js" as Engine

Item {
    id: root

    width: parent ? parent.width : 0
    height: parent ? parent.height : 0

    // Page stack depth.
    property int depth: Engine.getDepth()

    // The currently active page.
    property Item currentPage: null

    // The application toolbar
    property Item toolbar: null

    // Pushes a page on the stack.
    // The page can be defined as a component or an item.
    // If an item is used then the page will get re-parented.
    // Returns the page instance.
    function push(page) {
        return Engine.push(page);
    }

    // Pops a page off the stack.
    // If page is specified then the stack is unwound to that page.
    // Returns the page instance that was popped off the stack.
    function pop(page) {
        return Engine.pop(page);
    }

    // Replaces a page on the stack.
    // See push() for details.
    function replace(page) {
        return Engine.push(page, true);
    }

    // Clears the page stack.
    function clear() {
        return Engine.clear();
    }

    onVisibleChanged: {
        Engine.onVisibleChanged();
    }
    
    // Component for page slots.
    Component {
        id: slotComponent

        Item {
            id: slot

            width: parent ? parent.width : 0
            height: parent ? parent.height : 0

            // The states correspond to the different possible positions of the slot.
            state: "hidden"

            // The page held by this slot.
            property Item page: null
            // The owner of the page.
            property Item owner: null

            // Duration of transition animation (in ms)
            property int transitionDuration: 500

            // Performs a push enter transition.
            function pushEnter(replace, immediate) {
                if (!immediate) {
                    state = replace ? "front" : "right";
                }
                state = "";
                page.visible = true;
                if (root.visible && page.activated) {
                    page.activated();
                }
            }

            // Performs a push exit transition.
            function pushExit(replace, immediate) {
                state = immediate ? "hidden" : (replace ? "back" : "left");
                if (root.visible && page.deactivated) {
                    page.deactivated();
                }
                if (replace) {
                    slot.destroy(immediate ? 0 : transitionDuration);
                }
            }

            // Performs a pop enter transition.
            function popEnter(immediate) {
                if (!immediate) {
                    state = "left";
                }
                state = "";
                page.visible = true;
                if (root.visible && page.activated) {
                    page.activated();
                }
            }

            // Performs a pop exit transition.
            function popExit(immediate) {
                state = immediate ? "hidden" : "right";
                if (root.visible && page.deactivated) {
                    page.deactivated();
                }
                slot.destroy(immediate ? 0 : transitionDuration);
            }

            states: [
                // Start state for pop entry, end state for push exit.
                State {
                    name: "left"
                    PropertyChanges { target: slot; x: -width }
                },
                // Start state for push entry, end state for pop exit.
                State {
                    name: "right"
                    PropertyChanges { target: slot; x: width }
                },
                // Start state for replace entry.
                State {
                    name: "front"
                    PropertyChanges { target: slot; scale: 1.5; opacity: 0.0 }
                },
                // End state for replace exit.
                State {
                    name: "back"
                    PropertyChanges { target: slot; scale: 0.5; opacity: 0.0 }
                },
                // Inactive state.
                State {
                    name: "hidden"
                    PropertyChanges { target: slot; visible: false }
                }
            ]

            transitions: [
                // Pop entry and push exit transition.
                Transition {
                    from: ""; to: "left"; reversible: true
                    SequentialAnimation {
                        PropertyAnimation { properties: "x"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                        ScriptAction { script: if (state == "left") state = "hidden"; }
                    }
                },
                // Push entry and pop exit transition.
                Transition {
                    from: ""; to: "right"; reversible: true
                    SequentialAnimation {
                        PropertyAnimation { properties: "x"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                        ScriptAction { script: if (state == "right") state = "hidden"; }
                    }
                },
                // Replace entry transition.
                Transition {
                    from: "front"; to: "";
                    SequentialAnimation {
                        PropertyAnimation { properties: "scale,opacity"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                    }
                },
                // Replace exit transition.
                Transition {
                    from: ""; to: "back";
                    SequentialAnimation {
                        PropertyAnimation { properties: "scale,opacity"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                        ScriptAction { script: if (state == "back") state = "hidden"; }
                    }
                }
            ]

            // Called when the slot gets destroyed.
            Component.onDestruction: {
                if (state == "") {
                    // the page is active - deactivate it
                    if (root.visible && page.deactivated) {
                        page.deactivated();
                    }
                }
                if (owner != slot) {
                    // slot is not the owner of the page - re-parent back to original owner
                    page.visible = false;
                    page.parent = owner;
                }
            }

        }
    }

}
