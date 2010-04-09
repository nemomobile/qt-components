import Qt 4.6 as Qt
import Mx 1.0 as Mx
import "calculator.js" as CalcEngine

Mx.Window {
    width: 320; height: 240+headerBottom; 
    comboModel: Qt.ListModel{
        Qt.ListElement{
            content: 'Normal'
        }
        Qt.ListElement{
            content: 'Advanced'
        }
    }
    comboCurrent: 'Normal'

    function doOp(operation) { CalcEngine.doOperation(operation); }

    Qt.SystemPalette { id: palette }

    Qt.Column {
        x: 3; spacing: 10;
        y: headerBottom + 4

        Qt.Rectangle {
            id: container
            width: 314; height: 50
            border.color: palette.dark; color: palette.base

            Mx.Label {
                id: curNum
                font.bold: true; font.pointSize: 16
                color: palette.text
                anchors.right: container.right
                anchors.rightMargin: 5
                anchors.verticalCenter: container.verticalCenter
            }

            Mx.Label {
                id: currentOperation
                color: palette.text
                font.bold: true; font.pointSize: 16
                anchors.left: container.left
                anchors.leftMargin: 5
                anchors.verticalCenter: container.verticalCenter
            }
        }

        Qt.Item {
            width: 314; height: 160

            Qt.Item {
                id: basicButtons
                x: 55; width: 160; height: 160

                CalcButton { operation: "Bksp"; id: bksp; width: 67; opacity: 0 }
                CalcButton { operation: "C"; id: c; width: 76 }
                CalcButton { operation: "AC"; id: ac; x: 78; width: 76 }

                Qt.Grid {
                    id: numKeypad; y: 32; spacing: 2; columns: 3

                    CalcButton { operation: "7" }
                    CalcButton { operation: "8" }
                    CalcButton { operation: "9" }
                    CalcButton { operation: "4" }
                    CalcButton { operation: "5" }
                    CalcButton { operation: "6" }
                    CalcButton { operation: "1" }
                    CalcButton { operation: "2" }
                    CalcButton { operation: "3" }
                }

                Qt.Row {
                    y: 128; spacing: 2

                    CalcButton { operation: "0"; width: 50 }
                    CalcButton { operation: "."; x: 77; width: 50 }
                    CalcButton { operation: "="; id: equals; x: 77;  width: 102 }
                }

                Qt.Column {
                    id: simpleOperations
                    x: 156; y: 0; spacing: 2

                    CalcButton { operation: "x" }
                    CalcButton { operation: "/" }
                    CalcButton { operation: "-" }
                    CalcButton { operation: "+" }
                }
            }

            Qt.Grid {
                id: advancedButtons
                x: 350; spacing: 2; columns: 2; opacity: 0

                CalcButton { operation: "Abs" }
                CalcButton { operation: "Int" }
                CalcButton { operation: "MC" }
                CalcButton { operation: "Sqrt" }
                CalcButton { operation: "MR" }
                CalcButton { operation: "^2" }
                CalcButton { operation: "MS" }
                CalcButton { operation: "1/x" }
                CalcButton { operation: "M+" }
                CalcButton { operation: "+/-" }
            }
        }
    }

    states: Qt.State {
        name: "Advanced"; when: comboCurrent == 'Advanced'
        Qt.PropertyChanges { target: basicButtons; x: 0 }
        Qt.PropertyChanges { target: simpleOperations; y: 32 }
        Qt.PropertyChanges { target: bksp; opacity: 1 }
        Qt.PropertyChanges { target: c; x: 69; width: 67 }
        Qt.PropertyChanges { target: ac; x: 138; width: 67 }
        Qt.PropertyChanges { target: equals; width: 50 }
        Qt.PropertyChanges { target: advancedButtons; x: 210; opacity: 1 }
    }

    transitions: Qt.Transition {
        Qt.NumberAnimation { properties: "x,y,width"; easing.type: "OutBounce"; duration: 500 }
        Qt.NumberAnimation { properties: "opacity"; easing.type: "InOutQuad"; duration: 500 }
    }
}
