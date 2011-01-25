import Qt 4.7
import Qt.labs.components.native 1.0

ProgressBar {
   id: testTarget
   width: 640; height: 480

   minimumValue: 0
   maximumValue: 100
   value: 50
   indeterminate: false
}
