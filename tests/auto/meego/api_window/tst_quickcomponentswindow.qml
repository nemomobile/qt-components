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

import QtQuick 1.1
import com.nokia.meego 1.0

Window {
    id: rootWindow
  
    PageStack {
        id: pageStack
        anchors.top: parent.top
        width: parent.width
    }

    Component {
        id: pageComp
        
        Page {
            id:testPage
    
            function getOrientation() {
                return orientationText.text;
            }
    
            Label {
                id: orientationText
                objectName: "orientationTextLabel"
                anchors.centerIn: parent
                text: "XX"
                font.pixelSize: 24
            }
            
           Button {
              anchors.right: parent.right
                text: getOrientation();
                onClicked: {
                  
                    getOrientationLabelText();
                
                    if(screen.currentOrientation == Screen.Landscape) {
                        screen.allowedOrientations = Screen.PortraitInverted;
                    } else if(screen.currentOrientation == Screen.PortraitInverted) {
                            screen.allowedOrientations = Screen.LandscapeInverted;
                    } else if(screen.currentOrientation == Screen.LandscapeInverted) {
                            screen.allowedOrientations = Screen.Portrait;
                    } else if(screen.currentOrientation == Screen.Portrait) {
                            screen.allowedOrientations = Screen.Landscape;
                    }
                }
            }
            
            states: [
                State {
                    name: "inLandscape"
                    when: !rootWindow.inPortrait
                    PropertyChanges {
                        target: orientationText
                        text: "Landscape"
                    }
                },
                State {
                    name: "inPortrait"
                    when: rootWindow.inPortrait
                    PropertyChanges {
                        target: orientationText
                        text: "Portrait"
                    }
                }
            ]
        }
    }

    Component.onCompleted: {
        pageStack.push(pageComp);
    }

    function getOrientation() {
        if(screen.currentOrientation == Screen.Landscape) {
            return "Angle 0"
        } else if(screen.currentOrientation == Screen.PortraitInverted) {
            return "Angle 90"
        } else if(screen.currentOrientation == Screen.LandscapeInverted) {
            return "Angle 180";
        } else if(screen.currentOrientation == Screen.Portrait) {
            return "Angle 270"
        }    
    }

    function rotateToLandscape() {
        screen.allowedOrientations = Screen.Landscape;
    }
    
    function rotateToPortrait() {
        screen.allowedOrientations = Screen.Portrait;
    }
    
    function getOrientationLabelText() {
        console.log(pageStack.currentPage.getOrientation());
        return pageStack.currentPage.getOrientation();
    }
}
