#!/bin/sh
MAKE='make -j4'

cd `dirname $0`

rm -rf debian
mkdir debian
cd debian

mkdir -p "usr/share/applications"

desktop_file="usr/share/applications/qt-components.desktop"

echo "[Desktop Entry]" > "$desktop_file"
echo "Version=1.0" >> "$desktop_file"
echo "Name=qt-components" >> "$desktop_file"
echo "GenericName=QML calculator" >> "$desktop_file"
echo "Comment=QML calculator" >> "$desktop_file"
echo "Exec=/opt/qt-components/calculator" >> "$desktop_file"
echo "Terminal=false" >> "$desktop_file"
echo "Type=Application" >> "$desktop_file"

mkdir -p DEBIAN

control_file="DEBIAN/control"

echo "Package: qt-components" > "$control_file"
echo "Version: 1.0" >> "$control_file"
echo "Architecture: all" >> "$control_file"
echo "Maintainer: Qt developers" >> "$control_file"
echo "Description: Qt components test" >> "$control_file"

[ ! -f "../src/Makefile" ] && (cd ../src; qmake -r CONFIG+=meego)
(export INSTALL_ROOT=`pwd`; cd ../src; $MAKE install)

[ ! -f "../examples/meego/calculator" ] && (cd ../examples/meego/calculator; qmake; $MAKE)

mkdir -p opt/qt-components

cp -ar ../examples/meego/calculator/calculator opt/qt-components
cp -ar ../examples/meego/calculator/calculator.qml opt/qt-components
cp -ar ../examples/meego/calculator/Core opt/qt-components

cd ..
dpkg-deb --build debian
mv debian.deb qt-components.deb
