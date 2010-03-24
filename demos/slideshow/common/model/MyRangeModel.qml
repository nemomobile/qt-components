import Qt 4.6
import Components 1.0

RangeModel {
    id: model
    minimumValue: 0
    maximumValue: 100
    minimumPosition: 0
    position: knob.x + (knob.width / 2)
}
