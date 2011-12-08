/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QPointer>

#include "mthemeplugin.h"

MThemePlugin::MThemePlugin(QObject *parent)
    : QObject(parent)
{
    m_inverted = false;
    m_colorScheme = "1";
    m_colorString = "";
    m_selectionColor = "#4591ff";
}

MThemePlugin::~MThemePlugin()
{
}

bool MThemePlugin::isInverted() const
{
    return m_inverted;
}

void MThemePlugin::setInverted(bool inverted)
{
    if (m_inverted != inverted) {
        m_inverted = inverted;
        emit invertedChanged();
    }
}

QString MThemePlugin::colorScheme() const
{
    return m_colorScheme;
}

void MThemePlugin::setColorScheme(QString color)
{
    QString cs;
    QString sc;
    if (m_colorScheme != color) {
        m_colorScheme = color;
        emit colorSchemeChanged();
    }
    if (color == "2" || color == "lightGreen" || color == "green1") {
        cs = "color2-";
        sc = "#62B700";
    } else if (color == "3" || color == "green" || color == "green2") {
        cs = "color3-";
        sc = "#3D890C";
    } else if (color == "4" || color == "darkGreen" || color == "green3") {
        cs = "color4-";
        sc = "#347708";
    } else if (color == "5" || color == "darkestGreen" || color == "green4") {
        cs = "color5-";
        sc = "#306600";
    } else if (color == "6" || color == "lightBlue" || color == "blue1") {
        cs = "color6-";
        sc = "#870CBA";
    } else if (color == "7" || color == "blue" || color == "blue2") {
        cs = "color7-";
        sc = "#09A7CC";
    } else if (color == "8" || color == "darkBlue" || color == "blue3") {
        cs = "color8-";
        sc = "#0067BC";
    } else if (color == "9" || color == "darkestBlue" || color == "blue4") {
        cs = "color9-";
        sc = "#1C51AF";
    } else if (color == "10" || color == "darkPurple" || color == "purple2") {
        cs = "color10-";
        sc = "#6400BC";
    } else if (color == "11" || color == "purple" || color == "purple1") {
        cs = "color11-";
        sc = "#870CBA";
    } else if (color == "12" || color == "pink" || color == "pink2") {
        cs = "color12-";
        sc = "#CC09BA";
    } else if (color == "13" || color == "lightPink" || color == "pink1") {
        cs = "color13-";
        sc = "#E800A1";
    } else if (color == "14" || color == "lightOrange" || color == "orange1") {
        cs = "color14-";
        sc = "#EF5500";
    } else if (color == "15" || color == "orange" || color == "orange2") {
        cs = "color15-";
        sc = "#EA650A";
    } else if (color == "16" || color == "darkOrange" || color == "orange3") {
        cs = "color16-";
        sc = "#F77219";
    } else if (color == "17" || color == "darkYellow" || color == "yellow3") {
        cs = "color17-";
        sc = "#FF8500";
    } else if (color == "18" || color == "yellow" || color == "yellow2") {
        cs = "color18-";
        sc = "#ED9507";
    } else if (color == "19" || color == "lightYellow" || color == "yellow1") {
        cs = "color19-";
        sc = "#F2B111";
    } else {
        cs = "";
        sc = "#4591ff";
    }
    if (cs != m_colorString) {
        m_colorString = cs;
        emit colorStringChanged();
    }
    if (sc != m_selectionColor) {
        m_selectionColor = sc;
        emit selectionColorChanged();
    }
}

QString MThemePlugin::colorString() const
{
    return m_colorString;
}

QString MThemePlugin::selectionColor() const
{
    return m_selectionColor;
}

#include "moc_mthemeplugin.cpp"
