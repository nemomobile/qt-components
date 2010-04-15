#!/bin/sh
if [ $(uname) == "Darwin" ]; then
  DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
else
  LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
fi
qml -I modules/components -I demos/slideshow/model demos/slideshow/$1/main.qml
