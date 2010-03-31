import Qt 4.7
import Mx 1.0

Item {
    Column {
        spacing: 10;

        Mx.Combobox {
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