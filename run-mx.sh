#!/bin/sh
if [ $(uname) = "Darwin" ]; then
  DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
  qmlviewer -I modules/components -I modules/mx $@
else
  LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
  $QTDIR/bin/qmlviewer -I modules/components -I modules/mx $@
fi
