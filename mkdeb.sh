#!/bin/sh
MAKE='make -j4'

cd `dirname $0`

rm -rf debian
mkdir debian
cd debian

mkdir -p "usr/share/applications"

desktop_file_calculator="usr/share/applications/qt-components-calculator.desktop"
desktop_file_gallery="usr/share/applications/qt-components-gallery.desktop"

echo "[Desktop Entry]" > "$desktop_file_calculator"
echo "Version=1.0" >> "$desktop_file_calculator"
echo "Name=qt-components_calculator" >> "$desktop_file_calculator"
echo "GenericName=QML calculator" >> "$desktop_file_calculator"
echo "Comment=QML calculator" >> "$desktop_file_calculator"
echo "Exec=/opt/qt-components/calculator/calculator" >> "$desktop_file_calculator"
echo "Terminal=false" >> "$desktop_file_calculator"
echo "Type=Application" >> "$desktop_file_calculator"


echo "[Desktop Entry]" > "$desktop_file_gallery"
echo "Version=1.0" >> "$desktop_file_gallery"
echo "Name=QML Widget Gallery" >> "$desktop_file_gallery"
echo "GenericName=QML Widget Gallery" >> "$desktop_file_gallery"
echo "Comment=QML Widget Gallery" >> "$desktop_file_gallery"
echo "Exec=/opt/qt-components/gallery/gallery" >> "$desktop_file_gallery"
echo "Terminal=false" >> "$desktop_file_gallery"
echo "Type=Application" >> "$desktop_file_gallery"

mkdir -p DEBIAN

control_file="DEBIAN/control"

echo "Package: qt-components" > "$control_file"
echo "Version: 1.0" >> "$control_file"
echo "Architecture: all" >> "$control_file"
echo "Maintainer: Qt developers" >> "$control_file"
echo "Description: Qt components test" >> "$control_file"

[ ! -f "../src/Makefile" ] && (cd ../src; qmake -r CONFIG+=meego CONFIG+=contextsubscriber)
(export INSTALL_ROOT=`pwd`; cd ../src; $MAKE install)

[ ! -f "../examples/meego/Makefile" ] && (cd ../examples/meego/calculator; qmake; cd ../gallery; qmake)
(cd ../examples/meego/calculator; $MAKE; cd ../gallery; $MAKE)

mkdir -p opt/qt-components/calculator
mkdir -p opt/qt-components/gallery

cp -ar ../examples/meego/calculator/calculator opt/qt-components/calculator
cp -ar ../examples/meego/calculator/calculator.qml opt/qt-components/calculator
cp -ar ../examples/meego/calculator/Core opt/qt-components/calculator

cp -ar ../examples/meego/gallery/gallery opt/qt-components/gallery
cp -ar ../examples/meego/gallery/*.qml opt/qt-components/gallery

cd ..
dpkg-deb --build debian
mv debian.deb qt-components.deb
