 import Qt 4.7

 ListModel {
     id:widgetGallerySections 

     ListElement {
         name: "Application View"
	 source: "ApplicationView.qml"
     }
     ListElement {
         name: "Simple Widgets"
	 source: "SimpleWidgets.qml"
     }
     ListElement {
         name: "Buttons"
         source: "Buttons.qml"
     }
     ListElement {
         name: "Dialogs and Banners"
         source: "DialogsAndBanners.qml"
     }
     ListElement {
         name: "List, Grids and Popups"
         source: "ListGridsAndPopups.qml"
     } 
     ListElement {
         name: "User Input"
         source: "UserInput.qml"
     }
}
