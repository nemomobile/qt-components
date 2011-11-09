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

    property alias privateRect: rect

    onSelectionStartRectChanged: {
        Private.adjustPosition(contents);
    }
    onSelectionEndRectChanged: {
        Private.adjustPosition(contents);
    }

    Item {
        id: rect
        objectName: "selectionHandles"
        // fake baseline since the baseline of a font is not accessible in QML (except for anchors which doesn't work well here):
        property int fontBaseLine: textInput ? textInput.font.pixelSize * 0.16 : 0

        z: 1030 // Have the small selection handles above the big copy-paste bubble

        Image {
            id: leftSelectionImage
            objectName: "leftSelectionImage"
              property variant dragStart: Qt.point(0,0); // required for selection across multiple lines
              property int offset: -width/2;
              property int animationDuration: leftSelectionMouseArea.pressed ? 350 : 0
              x: selectionStartPoint.x + offset;
              y: selectionStartPoint.y + contents.selectionStartRect.height - 10 - rect.fontBaseLine; // vertical offset: 4 pixels
              source: platformStyle.leftSelectionHandle
              property bool pressed: false;

              MouseArea {
                  id: leftSelectionMouseArea
                  anchors.fill: parent
                  onPressed: {
                      if (Popup.isOpened(textInput)) {
                          Popup.close(textInput);
                      }
                      leftSelectionImage.dragStart = Qt.point( mouse.x, mouse.y );
                      leftSelectionImage.pressed = true;
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
                      Popup.open(textInput,textInput.positionToRectangle(textInput.cursorPosition));
                      leftSelectionImage.pressed = false;
                  }
              }

              states: [
                  State {
                      name: "normal"
                      when: !leftSelectionImage.pressed && !rightSelectionImage.pressed
                  },
                  State {
                      name: "pressed"
                      when: leftSelectionImage.pressed
                  },
                  State {
                      name: "otherpressed"
                      when: rightSelectionImage.pressed
                  }
              ]

              transitions: [
                  Transition {
                      from: "pressed"; to: "normal"
                      NumberAnimation {target: leftSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 0.0; to: 1.0; duration: 350}
                  },
                  Transition {
                      from: "normal"; to: "pressed"
                      NumberAnimation {target: leftSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 1.0; to: 0.0; duration: 350}
                  },
                  Transition {
                      from: "otherpressed"; to: "normal"
                      NumberAnimation {target: leftSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 0.7; to: 1.0; duration: 350}
                  },
                  Transition {
                      from: "normal"; to: "otherpressed"
                      NumberAnimation {target: leftSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 1.0; to: 0.7; duration: 350}
                  }
              ]
        }

        Image {
            id: rightSelectionImage
            objectName: "rightSelectionImage"
              property variant dragStart: Qt.point(0,0); // required for selection across multiple lines
              property int offset: -width/2;
              property int animationDuration: rightSelectionMouseArea.pressed ? 350 : 0
              x: selectionEndPoint.x + offset;
              y: selectionEndPoint.y + contents.selectionEndRect.height - 10 - rect.fontBaseLine; // vertical offset: 4 pixels
              source: platformStyle.rightSelectionHandle;
              property bool pressed: false;

              MouseArea {
                  id: rightSelectionMouseArea
                  anchors.fill: parent
                  onPressed: {
                      if (Popup.isOpened(textInput)) {
                          Popup.close(textInput);
                      }
                      rightSelectionImage.dragStart = Qt.point( mouse.x, mouse.y );
                      rightSelectionImage.pressed = true;
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
                      Popup.open(textInput,textInput.positionToRectangle(textInput.cursorPosition));
                      rightSelectionImage.pressed = false;
                 }
             }

              states: [
                  State {
                      name: "normal"
                      when: !leftSelectionImage.pressed && !rightSelectionImage.pressed
                  },
                  State {
                      name: "pressed"
                      when: rightSelectionImage.pressed
                  },
                  State {
                      name: "otherpressed"
                      when: leftSelectionImage.pressed
                  }
              ]

              transitions: [
                  Transition {
                      from: "pressed"; to: "normal"
                      NumberAnimation {target: rightSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 0.0; to: 1.0; duration: 350}
                  },
                  Transition {
                      from: "normal"; to: "pressed"
                      NumberAnimation {target: rightSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 1.0; to: 0.0; duration: 350}
                  },
                  Transition {
                      from: "otherpressed"; to: "normal"
                      NumberAnimation {target: rightSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 0.7; to: 1.0; duration: 350}
                  },
                  Transition {
                      from: "normal"; to: "otherpressed"
                      NumberAnimation {target: rightSelectionImage; property: "opacity";
                                    easing.type: Easing.InOutQuad;
                                    from: 1.0; to: 0.7; duration: 350}
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
            PropertyChanges { target: leftSelectionHandle; pressed: false; }
            PropertyChanges { target: rightSelectionHandle; pressed: false; }
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
            NumberAnimation {target: rect; property: "opacity";
                          easing.type: Easing.InOutQuad;
                          from: 0.0; to: 1.0; duration: 350}
        },
        Transition {
            from: "opened"; to: "closed"
            NumberAnimation {target: rect; property: "opacity";
                          easing.type: Easing.InOutQuad;
                          from: 1.0; to: 0.0; duration: 350}
        }
    ]
}
