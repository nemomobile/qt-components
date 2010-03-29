import Qt 4.7
import "mx" as Mx

Rectangle {

    width: 480
    height: 800

    FontLoader { id: droidSans; source: "mx/fonts/Droid-Sans/DroidSans.ttf" }
    FontLoader { id: droidSansBold; source: "mx/fonts/Droid-Sans/DroidSans-Bold.ttf" }
    Column {
        Mx.Label {
            text: 'Label';
        }
        Mx.Button {
            text: 'Button';
        }
        Row{
            spacing: 2
            height: slider.height
            Mx.Slider{width: 128; id: slider}
            Text{y: 2; text:'Slider'; opacity: slider.value}
        }
        Mx.Entry{hint: 'Fixed Width Entry'; width: 240;}
        Mx.Entry{hint: 'Crazy Icons!'; secret: true}//TODO
        Mx.Entry{hint: 'Secret Mode'; secret: true}
	Mx.Expander{text: 'Expander'; Mx.Label {width: 240; text: "I am the very model of a modern major general. I've information vegetable animal and mineral. I know the kings of England and I quote the fights historical, from matathon to waterloo in order categorical."; wrap:true}}
    }
}
