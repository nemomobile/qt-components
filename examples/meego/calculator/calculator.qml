/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import Qt 4.7
import com.meego 1.0
import "Core"
import "Core/calculator.js" as CalcEngine


Window {
    id: main

    property string rotateLeft: "\u2939"
    property string rotateRight: "\u2935"
    property string leftArrow: "\u2190"
    property string division : "\u00f7"
    property string multiplication : "\u00d7"
    property string squareRoot : "\u221a"
    property string plusminus : "\u00b1"

    function doOp(operation) { CalcEngine.doOperation(operation) }

    Page {

        Column {
            id: box;
            spacing: 8

            anchors { fill: parent; topMargin: 16; bottomMargin: 6; leftMargin: 6; rightMargin: 6 }

            Row {
                Display { id: display; width: box.width; height: 64 }
            }

            Column {
                id: column; spacing: 6

                property real h: ((box.height - 72) / 6) - ((spacing * (6 - 1)) / 6)
                property real w: (box.width / 4) - ((spacing * (4 - 1)) / 4)

                Row {
                    spacing: 6

                    Button {
                        id: rotateButton
                        width: column.w; height: column.h; color: 'purple'; text: rotateLeft
                    }
                    Button { width: column.w; height: column.h; color: 'purple'; text: leftArrow }
                    Button { width: column.w; height: column.h; color: 'purple'; text: "C" }
                    Button { width: column.w; height: column.h; color: 'purple'; text: "AC" }
                }

                Row {
                    spacing: 6
                    property real w: (box.width / 4) - ((spacing * (4 - 1)) / 4)

                    Button { width: column.w; height: column.h; color: 'green'; text: "mc" }
                    Button { width: column.w; height: column.h; color: 'green'; text: "m+" }
                    Button { width: column.w; height: column.h; color: 'green'; text: "m-" }
                    Button { width: column.w; height: column.h; color: 'green'; text: "mr" }
                }

                Grid {
                    id: grid; rows: 4; columns: 5; spacing: 6

                    property real w: (box.width / columns) - ((spacing * (columns - 1)) / columns)

                    Button { width: grid.w; height: column.h; text: "7"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "8"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "9"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: division }
                    Button { width: grid.w; height: column.h; text: squareRoot }
                    Button { width: grid.w; height: column.h; text: "4"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "5"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "6"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: multiplication }
                    Button { width: grid.w; height: column.h; text: "x^2" }
                    Button { width: grid.w; height: column.h; text: "1"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "2"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "3"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "-" }
                    Button { width: grid.w; height: column.h; text: "1/x" }
                    Button { width: grid.w; height: column.h; text: "0"; color: 'blue' }
                    Button { width: grid.w; height: column.h; text: "." }
                    Button { width: grid.w; height: column.h; text: plusminus }
                    Button { width: grid.w; height: column.h; text: "+" }
                    Button { width: grid.w; height: column.h; text: "="; color: 'red' }
                }
            }
        }
    }
}
