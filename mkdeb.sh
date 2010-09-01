#!/bin/sh
MAKE='make -j4'

cd `dirname $0`

rm -rf debian
mkdir debian
cd debian

mkdir -p "usr/share/applications"

desktop_file_calculator="usr/share/applications/qt-components-calculator.desktop"
echo "[Desktop Entry]" > "$desktop_file_calculator"
echo "Version=1.0" >> "$desktop_file_calculator"
echo "Name=qt-components_calculator" >> "$desktop_file_calculator"
echo "GenericName=QML calculator" >> "$desktop_file_calculator"
echo "Comment=QML calculator" >> "$desktop_file_calculator"
echo "Exec=/opt/qt-components/calculator/calculator" >> "$desktop_file_calculator"
echo "Terminal=false" >> "$desktop_file_calculator"
echo "Type=Application" >> "$desktop_file_calculator"


desktop_file_gallery="usr/share/applications/qt-components-gallery.desktop"
echo "[Desktop Entry]" > "$desktop_file_gallery"
echo "Version=1.0" >> "$desktop_file_gallery"
echo "Name=QML Widget Gallery" >> "$desktop_file_gallery"
echo "GenericName=QML Widget Gallery" >> "$desktop_file_gallery"
echo "Comment=QML Widget Gallery" >> "$desktop_file_gallery"
echo "Exec=/opt/qt-components/gallery/gallery" >> "$desktop_file_gallery"
echo "Terminal=false" >> "$desktop_file_gallery"
echo "Type=Application" >> "$desktop_file_gallery"

desktop_file_widgetgallery="usr/share/applications/qt-components-widgetgallery.desktop"
echo "[Desktop Entry]" > "$desktop_file_widgetgallery"
echo "Version=1.0" >> "$desktop_file_widgetgallery"
echo "Name=Widgets Gallery" >> "$desktop_file_widgetgallery"
echo "GenericName=Widgets Gallery" >> "$desktop_file_widgetgallery"
echo "Comment=Emulating Harmattan Widgets Gallery" >> "$desktop_file_widgetgallery"
echo "Exec=/opt/qt-components/widgetgallery/widgetgallery" >> "$desktop_file_widgetgallery"
echo "Terminal=false" >> "$desktop_file_widgetgallery"
echo "Type=Application" >> "$desktop_file_widgetgallery"

desktop_file_quiz="usr/share/applications/qt-components-quiz.desktop"
echo "[Desktop Entry]" > "$desktop_file_quiz"
echo "Version=1.0" >> "$desktop_file_quiz"
echo "Name=Quiz" >> "$desktop_file_quiz"
echo "GenericName=Quiz" >> "$desktop_file_quiz"
echo "Comment=Quiz" >> "$desktop_file_quiz"
echo "Exec=/opt/qt-components/quiz/quiz" >> "$desktop_file_quiz"
echo "Terminal=false" >> "$desktop_file_quiz"
echo "Type=Application" >> "$desktop_file_quiz"

desktop_file_pages="usr/share/applications/qt-components-pages.desktop"
echo "[Desktop Entry]" > "$desktop_file_pages"
echo "Version=1.0" >> "$desktop_file_pages"
echo "Name=Pages" >> "$desktop_file_pages"
echo "GenericName=Pages" >> "$desktop_file_pages"
echo "Comment=Pages" >> "$desktop_file_pages"
echo "Exec=/opt/qt-components/pages/pages" >> "$desktop_file_pages"
echo "Terminal=false" >> "$desktop_file_pages"
echo "Type=Application" >> "$desktop_file_pages"

mkdir -p DEBIAN

control_file="DEBIAN/control"

echo "Package: qt-components" > "$control_file"
echo "Version: 1.0" >> "$control_file"
echo "Architecture: all" >> "$control_file"
echo "Maintainer: Qt developers" >> "$control_file"
echo "Description: Qt components test" >> "$control_file"

[ ! -f "../src/Makefile" ] && (cd ../src; qmake -r CONFIG+=meego CONFIG+=contextsubscriber)
(export INSTALL_ROOT=`pwd`; cd ../src; $MAKE install)

[ ! -f "../examples/meego/Makefile" ] && (cd ../examples/meego/; qmake -r; $MAKE)

mkdir -p opt/qt-components/calculator
mkdir -p opt/qt-components/gallery
mkdir -p opt/qt-components/widgetgallery
mkdir -p opt/qt-components/quiz
mkdir -p opt/qt-components/pages

cp -ar ../examples/meego/calculator/calculator opt/qt-components/calculator/
cp -ar ../examples/meego/calculator/calculator.qml opt/qt-components/calculator/
cp -ar ../examples/meego/calculator/Core opt/qt-components/calculator/

cp -ar ../examples/meego/gallery/gallery opt/qt-components/gallery/
cp -ar ../examples/meego/gallery/*.qml opt/qt-components/gallery/

cp -ar ../examples/meego/widgetgallery/*.qml opt/qt-components/widgetgallery/
cp -ar ../examples/meego/widgetgallery/widgetgallery opt/qt-components/widgetgallery/
cp -ar ../examples/meego/widgetgallery/images opt/qt-components/widgetgallery/

cp -ar ../examples/meego/quiz/quiz opt/qt-components/quiz/
cp -ar ../examples/meego/quiz/*.qml opt/qt-components/quiz/

cp -ar ../examples/meego/pages/pages opt/qt-components/pages/
cp -ar ../examples/meego/pages/*.qml opt/qt-components/pages/

cd ..
dpkg-deb --build debian
mv debian.deb qt-components.deb
