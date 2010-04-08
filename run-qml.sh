#!/bin/sh
LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
qml -L modules/components $@
