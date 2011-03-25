:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
:: All rights reserved.
:: Contact: Nokia Corporation (qt-info@nokia.com)
::
:: This file is part of the Qt Components project on Qt Labs.
::
:: No Commercial Usage
:: This file contains pre-release code and may not be distributed.
:: You may use this file in accordance with the terms and conditions contained
:: in the Technology Preview License Agreement accompanying this package.
::
:: GNU Lesser General Public License Usage
:: Alternatively, this file may be used under the terms of the GNU Lesser
:: General Public License version 2.1 as published by the Free Software
:: Foundation and appearing in the file LICENSE.LGPL included in the
:: packaging of this file.  Please review the following information to
:: ensure the GNU Lesser General Public License version 2.1 requirements
:: will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
::
:: If you have questions regarding the use of this file, please contact
:: Nokia at qt-info@nokia.com.
::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

@echo off

set BUILD_TREE=%CD%
set SOURCE_TREE=%~dp0
cd /D %SOURCE_TREE%
set SOURCE_TREE=%CD%
cd /D %BUILD_TREE%

set BUILD_MEEGO_STYLE=no
set BUILD_SYMBIAN_STYLE=no
set BUILD_EXAMPLES=yes
set BUILD_EXTRAS=yes
set BUILD_TESTS=yes
set HAVE_MOBILITY=auto
set QMAKE_CONFIG=
set QMAKE_DEBUG=

set QMAKE=qmake
if not "%QTDIR%" == "" set QMAKE=%QTDIR%\bin\qmake

set QMAKE_CACHE=%BUILD_TREE%\.qmake.cache
if exist "%QMAKE_CACHE%" del /Q %QMAKE_CACHE%

set QMAKEFEATURES=%SOURCE_TREE%\features
if not exist "%BUILD_TREE%\features" mkdir "%BUILD_TREE%\features"

set CONFIG_PRF=%BUILD_TREE%\features\qt-components-config.prf
if exist "%CONFIG_PRF%" del /Q %CONFIG_PRF%

shift
:parse
if "%0" == ""               goto qmake
if "%0" == "-meego"         goto meego
if "%0" == "-symbian"       goto symbian
if "%0" == "-mobility"      goto mobility
if "%0" == "-no-mobility"   goto nomobility
if "%0" == "-make"          goto make
if "%0" == "-nomake"        goto nomake
if "%0" == "-config"        goto config
if "%0" == "-d"             goto debug
if "%0" == "-help"          goto help
if "%0" == "-h"             goto help

:unknown
echo Unknown option: %0
:clean
if exist "%QMAKE_CACHE%" del /Q %QMAKE_CACHE%
goto end

:meego
set BUILD_MEEGO_STYLE=yes
shift
goto parse

:symbian
set BUILD_SYMBIAN_STYLE=yes
shift
goto parse

:mobility
set HAVE_MOBILITY=yes
shift
goto parse

:nomobility
set HAVE_MOBILITY=no
shift
goto parse

:make
shift
if "%0" == "examples" goto examples
if "%0" == "extras" goto extras
if "%0" == "tests" goto tests
shift
goto unknown

:examples
set BUILD_EXAMPLES=yes
shift
goto parse

:extras
set BUILD_EXTRAS=yes
shift
goto parse

:tests
set BUILD_TESTS=yes
shift
goto parse

:nomake
shift
if "%0" == "examples" goto noexamples
if "%0" == "extras" goto noextras
if "%0" == "tests" goto notests
shift
goto unknown

:noexamples
set BUILD_EXAMPLES=no
shift
goto parse

:noextras
set BUILD_EXTRAS=no
shift
goto parse

:notests
set BUILD_TESTS=no
shift
goto parse

:config
shift
set QMAKE_CONFIG=%QMAKE_CONFIG% %0
shift
goto parse

:debug
set QMAKE_DEBUG=%QMAKE_DEBUG% -d
shift
goto parse

:help
echo.
echo Usage:  configure [-meego] [-symbian] [-config (config)]
echo         [-make (part)] [-nomake (part)] [-help]
echo.
echo Options:
echo.
echo    -meego ............ Build MeeGo Style
echo    -symbian .......... Build Symbian Style
echo    -config (config) .. Configuration options recognized by qmake
echo    -make (part) ...... Add part to the list of parts to be built at
echo                        make time (available parts: examples extras tests)
echo    -nomake (part) .... Exclude part from the list of parts to be built
echo.
echo Additional options:
echo.
echo A plus (+) denotes a default value that needs to be evaluated. If the
echo evaluation succeeds, the feature is included. Here is a short explanation
echo of each option:
echo.
echo    -no-mobility ............. Do not compile Qt Mobility support
echo +  -mobility ................ Compile Qt Mobility support
echo.
goto end

:qmake
if "%BUILD_MEEGO_STYLE%" == "yes" set QMAKE_CONFIG=%QMAKE_CONFIG% meego
if "%BUILD_SYMBIAN_STYLE%" == "yes" set QMAKE_CONFIG=%QMAKE_CONFIG% symbian3
if "%BUILD_EXAMPLES%" == "yes" set QMAKE_CONFIG=%QMAKE_CONFIG% examples
if "%BUILD_EXTRAS%" == "yes" set QMAKE_CONFIG=%QMAKE_CONFIG% extras
if "%BUILD_TESTS%" == "yes" set QMAKE_CONFIG=%QMAKE_CONFIG% tests

if "%HAVE_MOBILITY%" == "yes" set QMAKE_CONFIG=%QMAKE_CONFIG% mobility
if "%HAVE_MOBILITY%" == "no" set QMAKE_CONFIG=%QMAKE_CONFIG% no_mobility

echo CONFIG +=%QMAKE_CONFIG% > %CONFIG_PRF%
echo Q_COMPONENTS_SOURCE_TREE = %SOURCE_TREE:\=/% > %QMAKE_CACHE%
echo Q_COMPONENTS_BUILD_TREE = %BUILD_TREE:\=/% >> %QMAKE_CACHE%

echo.
echo Running qmake...
call %QMAKE% %QMAKE_DEBUG% -r \""CONFIG+=%QMAKE_CONFIG%"\" %SOURCE_TREE%\qt-components.pro
if errorlevel 1 echo ERROR: qmake run failed.

echo.
echo.
echo Qt Components build configuration:
echo Congiguration ....................%QMAKE_CONFIG%
echo MeeGo Style ...................... %BUILD_MEEGO_STYLE%
echo Symbian Style .................... %BUILD_SYMBIAN_STYLE%
if "%BUILD_SYMBIAN_STYLE%" == "no" goto nosymbian
  echo   Qt Mobility support ............ %HAVE_MOBILITY%
:nosymbian
echo   Extras ......................... %BUILD_EXTRAS%
echo Examples ......................... %BUILD_EXAMPLES%
echo Tests ............................ %BUILD_TESTS%
echo.
echo Qt Components is now configured for building. Just run '(n)make'.
echo.
:end
