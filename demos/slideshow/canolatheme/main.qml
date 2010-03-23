import Qt 4.6
import "../common"

MainApp {

    Settings {
        id: settingsDialog;
        state: "hide";
        onBack: {
            viewer.running = true;
            settingsDialog.state = "hide";
        }
    }
}