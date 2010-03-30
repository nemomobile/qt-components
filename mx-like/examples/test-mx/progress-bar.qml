import Qt 4.7
import Mx 1.0

Item {
    ProgressBar {
        x: 20
        y: 20
        width: 280

        SequentialAnimation on value {
            loops: Animation.Infinite
            NumberAnimation {
                from: 0
                to: 1
                duration: 5000
            }
        }
    }
}
