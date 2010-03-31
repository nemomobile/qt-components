import Qt 4.7
import Mx 1.0 as Mx

Item {
    Column {
        spacing: 10;

        Mx.ScrollBar {
            width: 200;
        }

        Mx.ScrollBar {
            vertical: true;
            height: 200;
        }
    }
}