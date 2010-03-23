#!/bin/sh
LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
qml -L plugin -L demos/slideshow/model demos/slideshow/$1/main.qml
