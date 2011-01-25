import Qt 4.7
import Qt.labs.components.native 1.0

Slider {
   id: testTarget
   width: 640; height: 480

   minimumValue: 0
   maximumValue: 100
   value: 50
// orientation: Qt.Vertical
// updateWhileDragging: true
// stepSize: 2.0
// pressed is a 'read-only' value
}
