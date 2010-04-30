#!/bin/sh
if [ $(uname) == "Darwin" ]; then
  DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
  qml -I modules/components -I modules/mx $@
else
  LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
  qml -I modules/components -I modules/mx $@
fi
