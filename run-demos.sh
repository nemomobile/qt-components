#!/bin/sh
if [ $(uname) == "Darwin" ]; then
  DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
else
  LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
fi
if [ -z $1 ]; then
THEME="canolatheme"
else
THEME=$1
fi
$QT_BUILD/bin/QMLViewer.app/Contents/MacOS/QMLViewer -I modules/components -I demos/slideshow/model demos/slideshow/$THEME/main.qml
