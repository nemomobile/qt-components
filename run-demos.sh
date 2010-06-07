#!/bin/sh
if [ -z $1 ]; then
THEME="canolatheme"
else
THEME=$1
fi

if [ $(uname) = "Darwin" ]; then
  DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
  $QT_BUILD/bin/QMLViewer.app/Contents/MacOS/QMLViewer -I modules/components -I demos/slideshow/model demos/slideshow/$THEME/main.qml
else
  LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
  $QTDIR/bin/qmlviewer -I modules/components -I demos/slideshow/model demos/slideshow/$THEME/main.qml
fi
