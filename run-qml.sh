#!/bin/sh
if [ $(uname) == "Darwin" ]; then
DYLD_LIBRARY_PATH=lib:$DYLD_LIBRARY_PATH
else
LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
fi
qmlviewer -I plugin/QtComponents $@
