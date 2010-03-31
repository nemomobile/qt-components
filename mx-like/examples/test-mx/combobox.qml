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
import Mx 1.0

Item {
    Column {
        x: 50;
        y: 50;
        spacing: 10;

        Combobox {
            model: cityModel;
            current: "London";

            ListModel {
                id: cityModel;
                ListElement {
                    content: "Strand";
                    icon: "";
                }
                ListElement {
                    content: "Fleet Street";
                    icon: "";
                }
                ListElement {
                    content: "Trafalgar Square";
                    icon: "";
                }
                ListElement {
                    content: "Leicester Square";
                    icon: "";
                }
                ListElement {
                    content: "Coventry Street";
                    icon: "";
                }
                ListElement {
                    content: "Piccadilly";
                    icon: "";
                }
            }
        }

        Combobox {
            model: weatherModel;

            ListModel {
                id: weatherModel;
                ListElement {
                    content: "Overcast";
                    icon: "images/weather-overcast.png";
                }
                ListElement {
                    content: "Snow";
                    icon: "images/weather-snow.png";
                }
                ListElement {
                    content: "Clear";
                    icon: "images/weather-clear.png";
                }
                ListElement {
                    content: "Cloudy";
                    icon: "images/weather-clouds.png";
                }
            }
        }
    }
}
