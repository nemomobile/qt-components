@echo off
SETLOCAL
set COMPONENTSPATH=%~dp0
set PATH=%COMPONENTSPATH%lib;%PATH%
qmlviewer -I plugin %*
