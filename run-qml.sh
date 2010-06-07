#!/bin/sh
LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
qmlviewer -I modules/components $@
