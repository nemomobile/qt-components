import QtQuick 1.1
import "." 1.0
import "Utils.js" as Utils
import "EditBubble.js" as Popup
import "SelectionHandles.js" as Private

Item {
    id: contents

    // Styling for the Label
    property Style platformStyle: SelectionHandlesStyle{}

    //Deprecated, TODO Remove this some day
    property alias style: contents.platformStyle

    property Item textInput: null

    property variant selectionStartRect: textInput ? textInput.positionToRectangle( textInput.selectionStart ) : Qt.rect(0,0,0,0);
    property variant selectionEndRect: textInput ? textInput.positionToRectangle( textInput.selectionEnd ) : Qt.rect(0,0,0,0);

    property variant selectionStartPoint: Qt.point(0,0)
    property variant selectionEndPoint: Qt.point(0,0)

    property alias leftSelectionHandle: leftSelectionImage
    property alias rightSelectionHandle: rightSelectionImage

    property bool ignoreClose: false
    property alias privateRect: rect

    onSelectionStartRectChanged: {
        Private.adjustPosition(contents);
    }
    onSelectionEndRectChanged: {
        Private.adjustPosition(contents);
    }

    Item {
        id: rect
        // fake baseline since the baseline of a font is not accessible in QML (except for anchors which doesn't work well here):
        property int fontBaseLine: textInput ? textInput.font.pixelSize * 0.16 : 0

        z: 1030 // Have the small selection handles above the big copy-paste bubble

        Image { id: leftSelectionImage
              property variant dragStart: Qt.point(0,0); // required for selection across multiple lines
              property int offset: 0;
              property int animationDuration: leftSelectionMouseArea.pressed ? 350 : 0
              x: selectionStartPoint.x + offset;
              y: selectionStartPoint.y + contents.selectionStartRect.height - 4 - rect.fontBaseLine; // vertical offset: 4 pixels
              source: ( textInput && leftSelectionMouseArea.pressed ) ? platformStyle.pressedLeftSelectionHandle : platformStyle.leftSelectionHandle;
              state: "normal";
              transform: Rotation { id: leftHandleRotation; origin.x: 0; origin.y: 0; axis { x: 0; y: 1; z: 0 } angle: 0}

              MouseArea {
                  id: leftSelectionMouseArea
                  anchors.fill: parent
                  onPressed: {
                      if (Popup.isOpened(textInput)) {
                          Popup.close(textInput);
                      }
                      leftSelectionImage.dragStart = Qt.point( mouse.x, mouse.y );
                  }
                  onPositionChanged: {
                      var pixelpos = mapToItem( textInput, mouse.x, mouse.y );
                      var ydelta = pixelpos.y - leftSelectionImage.dragStart.y;  // Determine the line distance
                      if ( ydelta < 0 ) ydelta = 0;  // Avoid jumpy text selection
                      var pos = textInput.positionAt(pixelpos.x, ydelta);
                      var h = textInput.selectionEnd;
                      if (pos >= h) {
                          pos = h - 1;  // Ensure at minimum one character between selection handles
                      }
                      textInput.select(pos,h); // Select by character
                  }
                  onReleased: {
                      // trim to word selection
                      ignoreClose = true;
                      var selectionEnd = textInput.selectionEnd;
                      textInput.cursorPosition = textInput.selectionStart;
                      textInput.moveCursorSelection ( selectionEnd, TextEdit.SelectWords );
                      Popup.open(textInput,textInput.positionToRectangle(textInput.cursorPosition));
                      ignoreClose = false;
                  }
              }
              states: [
                  State {
                      name: "normal"
                      PropertyChanges { target: leftSelectionImage;
                          offset: - width + 7; // horizontal offset: 7 pixels
                          mirror: false
                      }
                  },
                  State {
                      name: "mirrored"
                      PropertyChanges { target: leftSelectionImage;
                          offset: - 7; // horizontal offset: 7 pixels
                          mirror: true
                      }
                  }
              ]
              transitions: [
                  Transition {
                      from: "normal"; to: "mirrored"
                      ScriptAction {script: {
                              leftHandleRotation.origin.x = 7;
                          }
                      }
                      SequentialAnimation {
                          NumberAnimation {target: leftHandleRotation; property: "angle";
                                        from: 180.0; to: 0.0; duration: leftSelectionImage.animationDuration}
                      }
                  },
                  Transition {
                      from: "mirrored"; to: "normal"
                      SequentialAnimation {
                          ScriptAction {script: {
                                  leftHandleRotation.origin.x = leftSelectionImage.width - 7;
                                  leftSelectionImage.offset = -leftSelectionImage.width + 7;
                              }
                          }
                          NumberAnimation {target: leftHandleRotation; property: "angle";
                                        from: 180.0; to: 0.0; duration: leftSelectionImage.animationDuration}
                      }
                  }
              ]
        }

        Image { id: rightSelectionImage
              property variant dragStart: Qt.point(0,0); // required for selection across multiple lines
              property int offset: 0;
              property int animationDuration: rightSelectionMouseArea.pressed ? 350 : 0
              x: selectionEndPoint.x + offset;
              y: selectionEndPoint.y + contents.selectionEndRect.height - 4 - rect.fontBaseLine; // vertical offset: 4 pixels
              source: ( textInput && rightSelectionMouseArea.pressed ) ? platformStyle.pressedRightSelectionHandle : platformStyle.rightSelectionHandle;
              state: "normal";
              transform: Rotation { id: rightHandleRotation; origin.x: 0; origin.y: 0; axis { x: 0; y: 1; z: 0 } angle: 0}

              MouseArea {
                  id: rightSelectionMouseArea
                  anchors.fill: parent
                  onPressed: {
                      if (Popup.isOpened(textInput)) {
                          Popup.close(textInput);
                      }
                      rightSelectionImage.dragStart = Qt.point( mouse.x, mouse.y );
                  }
                  onPositionChanged: {
                      var pixelpos = mapToItem( textInput, mouse.x, mouse.y );
                      var ydelta = pixelpos.y - rightSelectionImage.dragStart.y;  // Determine the line distance
                      if ( ydelta < 0 ) ydelta = 0;  // Avoid jumpy text selection
                      var pos = textInput.positionAt(pixelpos.x, ydelta);
                      var h = textInput.selectionStart;
                      if (pos <= h) {
                          pos = h + 1;  // Ensure at minimum one character between selection handles
                      }
                      textInput.select(h,pos); // Select by character
                 }
                 onReleased: {
                      // trim to word selection
                      ignoreClose = true;
                      var selectionEnd = textInput.selectionEnd;
                      textInput.cursorPosition = textInput.selectionStart;
                      if (textInput == null) return;
                      textInput.moveCursorSelection ( selectionEnd, TextEdit.SelectWords );
                      Popup.open(textInput,textInput.positionToRectangle(textInput.cursorPosition));
                      ignoreClose = false;
                 }
             }
             states: [
                 State {
                     name: "normal"
                     PropertyChanges { target: rightSelectionImage;
                         offset: - 7; // horizontal offset: 7 pixels
                         mirror: false;
                     }
                 },
                 State {
                     name: "mirrored"
                     PropertyChanges { target: rightSelectionImage;
                         offset: - width + 7; // horizontal offset: 7 pixels
                         mirror: true
                     }
                 }
             ]
             transitions: [
                 Transition {
                     from: "mirrored"; to: "normal"
                     ScriptAction {script: {
                             rightHandleRotation.origin.x = 7;
                         }
                     }
                     SequentialAnimation {
                         NumberAnimation {target: rightHandleRotation; property: "angle";
                                       from: 180.0; to: 0.0; duration: rightSelectionImage.animationDuration}
                     }
                 },
                 Transition {
                     from: "normal"; to: "mirrored"
                     SequentialAnimation {
                         ScriptAction {script: {
                                 rightHandleRotation.origin.x = rightSelectionImage.width - 7;
                                 rightSelectionImage.offset = -rightSelectionImage.width + 7;
                             }
                         }
                         NumberAnimation {target: rightHandleRotation; property: "angle";
                                       from: 180.0; to: 0.0; duration: rightSelectionImage.animationDuration}
                     }
                 }
             ]
        }
    }

    Connections {
        target: Utils.findFlickable(textInput)
        onContentXChanged: Private.adjustPosition(contents)
        onContentYChanged: Private.adjustPosition(contents)
        onMovementEnded: Popup.open(textInput,textInput.positionToRectangle(textInput.cursorPosition));

    }

    Connections {
        target: screen
        onCurrentOrientationChanged: Private.adjustPosition(contents)
    }

    function findWindowRoot() {
        var item = Utils.findRootItem(contents, "windowRoot");
        if (item.objectName != "windowRoot")
            item = Utils.findRootItem(contents, "pageStackWindow");
        return item;
    }

    Connections {
       target: findWindowRoot();
       ignoreUnknownSignals: true
       onOrientationChangeFinished: {
           Private.adjustPosition(contents)
       }
    }

    state: "closed"

    states: [
        State {
            name: "opened"
            ParentChange { target: rect; parent: Utils.findRootItem(textInput); }
            PropertyChanges { target: rect; visible: true; }
        },
        State {
            name: "closed"
            ParentChange { target: rect; parent: contents; }
            PropertyChanges { target: rect; visible: false; }
        }
    ]

    transitions: [
        Transition {
            from: "closed"; to: "opened"
            SequentialAnimation {
                NumberAnimation {target: rect; property: "opacity";
                              from: 0.0; to: 1.0; duration: 350}
            }
        },
        Transition {
            from: "opened"; to: "closed"
            SequentialAnimation {
                NumberAnimation {target: rect; property: "opacity";
                              from: 1.0; to: 0.0; duration: 350}
            }
        }
    ]
}
