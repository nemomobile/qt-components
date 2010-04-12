#!/bin/sh
DYLD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
$QT_BUILD/bin/qml.app/Contents/MacOS/qml -L modules/components -L demos/slideshow/model demos/slideshow/$1/main.qml
