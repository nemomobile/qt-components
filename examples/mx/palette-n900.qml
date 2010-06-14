import Qt 4.7
import Mx 1.0 as Mx

Mx.Window {

    width: 800
    height: 480

    FontLoader { id: droidSans; source: "fonts/Droid-Sans/DroidSans.ttf" }
    FontLoader { id: droidSansBold; source: "fonts/Droid-Sans/DroidSans-Bold.ttf" }
    Row {
        spacing: 10
        x: 10
        y: parent.headerBottom + 10
        Column {
            spacing: 10


            Mx.Label {
                text: 'Label';
            }
            Mx.Button {
                text: 'Button';
                tooltipText: 'This is a button that you should not click as it does nothing.'
                z:2
            }

            Mx.Button {
                text: 'Toggle Button'
                checkable: true
                onClicked: {
                    console.log("Toggle Button " + (checked ? '(checked)' : '(not checked)'));
                }
            }

            Row{
                spacing: 2
                height: slider.height
                Mx.Slider{width: 128; id: slider}
                Text{y: 2; text:'Slider'; opacity: slider.value}
            }
            Mx.Entry{hint: 'Fixed Width Entry'; width: 240;}
            Mx.Entry{hint: 'Search...';
                leftIconSource: '../../modules/mx/Mx/images/edit-find.png';
                onLeftIconClicked: text='Hello world';
                rightIconSource: '../../modules/mx/Mx/images/edit-clear.png';
                onRightIconClicked: text='';
            }
            Mx.Entry{hint: 'Secret Mode'; secret: true}
            Mx.Expander{text: 'Expander'; Mx.Label {width: 240; text: "I am the very model of a modern major general. I've information vegetable animal and mineral. I know the kings of England and I quote the fights historical, from matathon to waterloo in order categorical.";
                    wrapMode:Text.WordWrap}}

        }
        Column {

            Mx.ProgressBar {
                width: 279;
                minimum: 0
                maximum:100
                SequentialAnimation on progress {
                    loops: Animation.Infinite;
                    NumberAnimation {
                        from: 0;
                        to: 100;
                        duration: 25000;
                    }
                }
            }

            Mx.Toggle {
                onActiveChanged: { console.log("Toggle active is " + active); }
            }
            Mx.ButtonGroup {
                id: group
                width: col.width
                height: col.height
                // allowNoChecked: true

                Column {
                    id: col

                    Mx.Button {
                        text: "test1"
                        checkable: true
                        checked: true
                    }
                    Mx.Button {
                        text: "test2"
                        checkable: true
                    }

                    Row {
                        Mx.Button {
                            text: "test3"
                            checkable: true
                        }
                        Mx.Button {
                            text: "test4"
                            checkable: true
                            checked: true
                        }
                    }

                    Mx.Button {
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

            Mx.Combobox {
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

            Mx.Combobox {
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
}
