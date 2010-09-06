 import Qt 4.7

 ListModel {
     id:widgetGallerySections 

     ListElement {
         name: "Application View"
	 source: "gallery/ApplicationView.qml"
     }
     ListElement {
         name: "Simple Widgets"
	 source: "gallery/SimpleWidgets.qml"
     }
     ListElement {
         name: "Buttons"
         source: "gallery/Buttons.qml"
     }
     ListElement {
         name: "Dialogs and Banners"
         source: "gallery/DialogsAndBanners.qml"
     }
     ListElement {
         name: "List, Grids and Popups"
         source: "gallery/ListGridsAndPopups.qml"
     } 
     ListElement {
         name: "User Input"
         source: "gallery/UserInput.qml"
     }
}
