import Qt 4.7
import "mx" as Mx

Rectangle {

    width: 480
    height: 800

    FontLoader { id: droidSans; source: "mx/fonts/Droid-Sans/DroidSans.ttf" }
    FontLoader { id: droidSansBold; source: "mx/fonts/Droid-Sans/DroidSans-Bold.ttf" }
    Column {
        spacing: 10
        x: 10

        Mx.Label {
            text: 'Label';
        }
        Mx.Button {
            text: 'Button';
            tooltipText: 'This is a button that you should not click as it does nothing.'
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
            leftIconSource: 'images/edit-find.png'; 
            onLeftIconClicked: text='Hello world';
            rightIconSource: 'images/edit-clear.png';
            onRightIconClicked: text='';
        }
        Mx.Entry{hint: 'Secret Mode'; secret: true}
	Mx.Expander{text: 'Expander'; Mx.Label {width: 240; text: "I am the very model of a modern major general. I've information vegetable animal and mineral. I know the kings of England and I quote the fights historical, from matathon to waterloo in order categorical."; wrap:true}}
        Mx.ProgressBar {
            width: 279;
            SequentialAnimation on value {
                loops: Animation.Infinite;
                NumberAnimation {
                    from: 0;
                    to: 100;
                    duration: 5000;
                }
            }
        }

        Mx.Toggle {
            onActiveChanged: { console.log("Toggle active is " + active); }
        }
    }
}
