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

import Qt 4.7
import Qt.labs.components 1.0
import com.meego.themebridge 1.0

// Background dimming
Rectangle {
  id: background

  Component.onCompleted: init()

  property bool modal: true
  property double dimm: 0.8

  opacity: 0.0
  color: "black"
  state: 'hidden'

  //eat mouse events inside the background
  MouseArea {
    id: mouseEventEater 
    anchors.fill: parent
    enabled: background.modal
  }

  function init() {
      if (modal) {
        var root = getRoot();
        var globalXY;
        if (root != null) { 
           globalXY = mapFromItem(root, root.x, root.y);
           width = root.width; height = root.height;
        } else { 
           globalXY = mapFromItem(null, 0, 0);
        }
        x = globalXY.x; y = globalXY.y;
      } else {
        anchors.fill = parent;
      }
  }

  function getRoot() {
      var next = parent;
      if (next != null) {
        while (next.parent) {
          next = next.parent;
        }
      }
      return next;
  }
  
    states: [
      State {
        name: "visible"
        PropertyChanges {
          target: background
          opacity: background.dimm
        }
      },
      State {
        name: "hidden"
        PropertyChanges {
           target: background
           opacity: 0.0
        }
      }
    ]
    
   transitions: [
     Transition {
        NumberAnimation {properties: "opacity"; duration: 900}
    }
   ]

}


    
