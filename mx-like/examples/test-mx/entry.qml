/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
import Qt 4.7
import Mx 1.0 as Mx

Item {
    Column {
        spacing: 10;

        Mx.Entry {
            width: 240;
            text: "Hello World!";
            hint: "Fixed Width Entry";
        }
        Mx.Entry {
            hint: "Search...";
            leftIconSource: "images/edit-find.png";
            onLeftIconClicked: text="Hello world";
            rightIconSource: "images/edit-clear.png";
            onRightIconClicked: text="";
        }

        Mx.Entry {
            id: textSetable;
            hint: "hint hint...";
        }
        Row {
            spacing: 10;
            Mx.Button {
                text: "Set";
                onClicked: textSetable.text = "Here is some text";
            }
            Mx.Button {
                text: "Clear";
                onClicked: textSetable.text = "";
            }
        }

        Mx.Entry {
            hint: "Secret!";
            secret: true;
        }
    }
}