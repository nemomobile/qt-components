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
import Qt.labs.Mx 1.0

Window {

    width: 480
    height: 800

    //FontLoader { id: droidSans; source: "fonts/Droid-Sans/DroidSans.ttf" }
    //FontLoader { id: droidSansBold; source: "fonts/Droid-Sans/DroidSans-Bold.ttf" }
    Column {
        spacing: 10
        x: 10
        y: parent.headerBottom

        Label {
            text: 'Label';
        }
        Button {
            text: 'Button';
            tooltipText: 'This is a button that you should not click as it does nothing.'
            z:2
        }

        Button {
            text: 'Toggle Button'
            checkable: true
            onClicked: {
                console.log("Toggle Button " + (checked ? '(checked)' : '(not checked)'));
            }
        }

        Row{
            spacing: 2
            height: slider.height
            Slider{width: 128; id: slider}
            Text{y: 2; text:'Slider'; opacity: slider.value}
        }
        Entry{hint: 'Fixed Width Entry'; width: 240;}
        Entry{hint: 'Search...';
            //leftIconSource: '../../modules/mx/Mx/images/edit-find.png'; //###
            onLeftIconClicked: text='Hello world';
            //rightIconSource: '../../modules/mx/Mx/images/edit-clear.png'; //###
            onRightIconClicked: text='';
        }
        Entry{hint: 'Secret Mode'; secret: true}
        Expander{text: 'Expander'; Label {width: 240; text: "I am the very model of a modern major general. I've information vegetable animal and mineral. I know the kings of England and I quote the fights historical, from matathon to waterloo in order categorical.";
            wrapMode:Text.WordWrap}}
        ProgressBar {
            width: 279;
            minimum: 0
            maximum:100
            SequentialAnimation on progress {
                loops: Animation.Infinite;
                NumberAnimation {
                    from: 0;
                    to: 100;
                    duration: 5000;
                }
            }
        }

        Toggle {
            onActiveChanged: { console.log("Toggle active is " + active); }
        }

        ButtonGroup {
            id: group
            width: col.width
            height: col.height
            // allowNoChecked: true

            Column {
                id: col

                Button {
                    text: "test1"
                    checkable: true
                    checked: true
                }
                Button {
                    text: "test2"
                    checkable: true
                }

                Row {
                    Button {
                        text: "test3"
                        checkable: true
                    }
                    Button {
                        text: "test4"
                        checkable: true
                        checked: true
                    }
                }

                Button {
                    text: "Which one is checked?"
                    onClicked: {
                        // ### find proper way to identify NULL pointers in C++ pointer properties
                        if (group.checkedItem.toString() !== "null") {
                            print(group.checkedItem.text);
                        } else {
                            print("No one is checked!");
                        }
                    }
                }
            }
        }

        Combobox {
            model: cityModel;
            current: "London";
            z:2

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
/*
        ScrollBar {
            state: "horizontal"
            width: 200
        }
        ScrollBar {
            state: "vertical"
            height: 200
        }
*/
    }
}
