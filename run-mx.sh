#!/bin/sh
if [ $(uname) == "Darwin" ]; then
  DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
  qml -L modules/components -L modules/mx $@
else
  LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
  qml -L modules/components -L modules/mx $@
fi
