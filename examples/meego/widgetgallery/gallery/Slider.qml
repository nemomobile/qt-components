import QtQuick 1.0
import com.meego 1.0
import com.meego.themebridge 1.0 as ThemeBridge // TODO: export Icon

Page {
    title: "Slider"

    // TODO: sections headers are being done manually here, not according to the style.

    Flickable {
        anchors.fill: parent
        contentHeight: contents.height + 10

        Column {
            id: contents
            y: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10

            Label {
                width: parent.width
                text: "Slider is used for continuous set of values, among which user can choose one value.\n\nSeekbar is a special type of Slider used for displaying playback status for multimedia content.\n"
                wrapMode: Text.WordWrap
            }

            Rectangle {
                width: parent.width
                height: ageLabel.height + 10
                color: "black"
                Label {
                    id: ageLabel
                    x: 10
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    text: "Personal data"
                }
            }

            Rectangle {
                color: "white"
                width: parent.width
                height: ageSlider.height

                Label {
                    id: msg
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    text: "Please select your age:"
                }
                Label {
                    id: minAge
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: msg.right
                    text: ageSlider.minimumValue
                }
                Slider {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: minAge.right
                    anchors.right: maxAge.left
                    id: ageSlider
                }
                Label {
                    id: maxAge
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    text: ageSlider.maximumValue
                }
            }

            Rectangle {
                width: parent.width
                height: playerLabel.height + 10
                color: "black"
                Label {
                    id: playerLabel
                    x: 10
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    text: "Player"
                }
            }

            Rectangle {
                color: "white"
                width: parent.width
                height: Math.max(playerSlider.height, playButton.height);

                IconButton {
                    id: playButton
                    iconId: "icon-m-common-play"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                }

                Label {
                    id: playerMin
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: playButton.right
                    text: playerSlider.formatValue(playerSlider.minimumValue)
                }

                // TODO: replace with the SeekBar
                Slider {
                    id: playerSlider

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: playerMin.right
                    anchors.right: playerMax.left
                    stepSize: 0

                    maximumValue: 123

                    function formatValue(v) {
                        v = Math.round(v);
                        var seconds = Math.floor(v % 60);
                        var minutes = Math.floor(v / 60);

                        // :-P
                        if (seconds < 10)
                            seconds = "0" + seconds;

                        return minutes + ":" + seconds
                    }
                }

                Label {
                    id: playerMax
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    text: playerSlider.formatValue(playerSlider.maximumValue)
                }
            }

            Rectangle {
                width: parent.width
                height: brightnessLabel.height + 10
                color: "black"
                Label {
                    id: brightnessLabel
                    x: 10
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    text: "Brightness"
                }
            }

            Rectangle {
                color: "white"
                width: parent.width
                height: Math.max(brightnessSlider.height, weak.height)

                ThemeBridge.Icon {
                    id: weak
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    iconId: "icon-m-common-strength1"
                }

                Slider {
                    id: brightnessSlider
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: weak.right
                    anchors.right: strong.left

                    function formatValue(v) {
                        return Math.round(v) + "%"
                    }
                }

                ThemeBridge.Icon {
                    id: strong
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    iconId: "icon-m-common-strength5"
                }
            }
        }
    }
}
