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

// The ToolBar is a container for toolbar items such as ToolItem or ToolButton.

import Qt 4.7
import com.meego.themebridge 1.0

Item {
    id: root
    
    width: parent ? parent.width : 0
    height: bgImage.height

    // Duration of transition animation (in ms)
    property int transitionDuration: 500

    // Toolbar background.
    BorderImage {
        id: bgImage
        width: root.width
        border.left: 10
        border.right: 10
        border.bottom: 10
        source: "image://theme/meegotouch-navigationbar-landscape-background"
    }

    Item {
        id: slotContainer
        width: root.width
        height: bgImage.height
    }

    states: [
        // Inactive state.
        State {
            name: "hidden"
            PropertyChanges { target: root; height: 0 }
        }
    ]

    transitions: [
        // Transition between active and inactive states.
        Transition {
            from: ""; to: "hidden"; reversible: true
            SequentialAnimation {
                PropertyAnimation { properties: "height"; easing.type: Easing.InOutExpo; duration: transitionDuration }
            }
        }
    ]

    // The current set of tools.
    property Item tools: null

    onToolsChanged: {
        _performTransition(_transition || transition);
        _transition = undefined;
    }

    // The transition type. One of the following:
    //      set         an instantaneous change (default)
    //      push        follows page stack push animation
    //      pop         follows page stack pop animation
    //      replace     follows page stack replace animation
    property string transition: "set"

    // The currently displayed slot; null if none.
    property Item _currentSlot: null

    // Alternating slots used for transitions.
    property Item _slotA: null
    property Item _slotB: null

    // The transition to perform next.
    property variant _transition

    // Sets the tools with a transition.
    function setTools(tools, transition) {
        root.state = tools ? "" : "hidden";
        _transition = transition;
        root.tools = tools;
    }

    // Performs a transition between tools in the toolbar.
    function _performTransition(transition) {
        // lazily create slots if they have not been created
        if (!_currentSlot) {
            _slotA = slotComponent.createObject(slotContainer);
            _slotB = slotComponent.createObject(slotContainer);
            _currentSlot = _slotB;
        }

        // no transition if the tools are unchanged
        if (_currentSlot.tools == tools) {
            return;
        }

        // select slot states based on the transition animation
        var transitions = {
            "set":      { "new": "",        "old": "hidden" },
            "push":     { "new": "right",   "old": "left" },
            "pop":      { "new": "left",    "old": "right" },
            "replace":  { "new": "front",   "old": "back" }
        };
        var animation = transitions[transition];

        // initialize the free slot
        var slot = _currentSlot == _slotA ? _slotB : _slotA;
        slot.state = "hidden";
        if (tools) {
            slot.tools = tools;
            slot.owner = tools.parent;
            tools.parent = slot;
            tools.visible = true;
        }

        // perform transition
        _currentSlot.state = animation["old"];
        if (tools) {
            slot.state = animation["new"];
            slot.state = "";
        }

        _currentSlot = slot;
    }

    // Component for toolbar slots.
    Component {
        id: slotComponent

        Item {
            id: slot

            width: parent ? parent.width : 0
            height: parent ? parent.height : 0

            // The states correspond to the different possible positions of the slot.
            state: "hidden"

            // The tools held by this slot.
            property Item tools: null
            // The owner of the tools.
            property Item owner: null

            states: [
                // Start state for pop entry, end state for push exit.
                State {
                    name: "left"
                    PropertyChanges { target: slot; x: -50; opacity: 0.0 }
                },
                // Start state for push entry, end state for pop exit.
                State {
                    name: "right"
                    PropertyChanges { target: slot; x: 50; opacity: 0.0 }
                },
                // Start state for replace entry.
                State {
                    name: "front"
                    PropertyChanges { target: slot; scale: 1.25; opacity: 0.0 }
                },
                // End state for replace exit.
                State {
                    name: "back"
                    PropertyChanges { target: slot; scale: 0.85; opacity: 0.0 }
                },
                // Inactive state.
                State {
                    name: "hidden"
                    PropertyChanges { target: slot; visible: false }
                    StateChangeScript {
                        script: {
                            if (slot.tools) {
                                // re-parent back to original owner
                                tools.visible = false;
                                tools.parent = owner;

                                // reset slot
                                slot.tools = slot.owner = null;
                            }
                        }
                    }
                }
            ]

            transitions: [
                // Pop entry and push exit transition.
                Transition {
                    from: ""; to: "left"; reversible: true
                    SequentialAnimation {
                        PropertyAnimation { properties: "x,opacity"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                        ScriptAction { script: if (state == "left") state = "hidden"; }
                    }
                },
                // Push entry and pop exit transition.
                Transition {
                    from: ""; to: "right"; reversible: true
                    SequentialAnimation {
                        PropertyAnimation { properties: "x,opacity"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                        ScriptAction { script: if (state == "right") state = "hidden"; }
                    }
                },
                Transition {
                    // Replace entry transition.
                    from: "front"; to: "";
                    SequentialAnimation {
                        PropertyAnimation { properties: "scale,opacity"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                    }
                },
                Transition {
                    // Replace exit transition.
                    from: ""; to: "back";
                    SequentialAnimation {
                        PropertyAnimation { properties: "scale,opacity"; easing.type: Easing.InOutExpo; duration: transitionDuration }
                        ScriptAction { script: if (state == "back") state = "hidden"; }
                    }
                }
            ]

        }
    }

}
