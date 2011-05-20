/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

import QtQuick 1.0
import Qt.labs.components.native 1.0

Loader {
    id: customLoader

    property bool debug: true
    property string path
    property string fileName

    //![Properties]
    property int attempt: 0
    property int largerDimension: Math.max(screen.displayWidth, screen.displayHeight)
    property int smallerDimension: Math.min(screen.displayWidth, screen.displayHeight)
    property int roundedDpi: Math.round(screen.dpi / 10) * 10
    property string mySource: path + "/" + largerDimension + "x" + smallerDimension + "/" + roundedDpi + "/" + fileName;
    //![Properties]

    signal loadError

    //![Display Categories]
    function displayCategory() {
        switch (screen.displayCategory) {
        case Screen.Small:
            return "Small";
        case Screen.Normal:
            return "Normal";
        case Screen.Large:
            return "Large";
        default:
            return "ExtraLarge";
        }
    }
    //![Display Categories]

    //![Density Categories]
    function densityCategory() {
        switch (screen.density) {
        case Screen.Low:
            return "Low";
        case Screen.Medium:
            return "Medium";
        case Screen.High:
            return "High";
        default:
            return "ExtraHigh";
        }
    }
    //![Density Categories]

    //![Loading]
    onMySourceChanged: {
        attempt = 0;
        if (customLoader.smallerDimension > 0 && customLoader.largerDimension > 0)
            source = mySource;
    }

    onStatusChanged: {
        if (customLoader.status == Loader.Error) {
            customLoader.attempt++;
            switch (customLoader.attempt) {
            case 1:
                source = path + "/" + largerDimension + "x" + smallerDimension + "/" + fileName;
                break;
            case 2:
                source = path + "/" + displayCategory() + "/" + densityCategory() + "/" + fileName;
                break;
            case 3:
                source = path + "/" + displayCategory() + "/" + fileName;
                break;
            case 4:
                source = path + "/" + fileName;
                break;
            default:
                customLoader.loadError();
                source = "";
            }
        } else {
            if (debug) console.log("CustomLoader: successfully loaded file: " + source);
        }
    }
    //![Loading]
}
