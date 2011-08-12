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

import QtQuick 1.1
import QtQuickTest 1.0
import com.nokia.extras 1.1

TestCase {
    name: "InfoBanner"

    // Switch
    InfoBanner {
        id: sb
    }

    // TODO Why do we test QML functionality? Just for the CIS?

    function test_property_text() {
        // change text
        var text = 'The quick brown fox jumps over the lazy dog';
        sb.text = text;
        compare(sb.text, text, 'compare variable assignment 1');

        // change text - exceed 3 lines
        text = 'The quick brown fox jumps over the lazy dog\nThe quick brown fox jumps over the lazy dog\nThe quick brown fox jumps over the lazy dog\nThe quick brown fox jumps over the lazy dog';
        sb.text = text;
        compare(sb.text, text, 'compare variable assignment 2');

        // unicode text
        text = '手机上网输入网址';
        sb.text = text;
        compare(sb.text, text, 'compare variable assignment 3');
    }
/*
    function test_property_iconSource() {
        // change icon source
        sb.iconSource = '../tests/assets/system_banner_thumbnail.png';
        compare(sb.iconSource, '../tests/assets/system_banner_thumbnail.png', 'compare variable assignment 1');

        // invalid icon source
        sb.iconSource = '../tests/assets/invalid.png';
        compare(sb.iconSource, '../tests/assets/invalid.png', 'compare variable assignment 2');
    }
*/

    function test_property_margins() {
        var margin = 20;
        sb.leftMargin = margin;
        sb.topMargin = margin;
        compare(sb.x, margin, 'compare variable assignment 1');
        compare(sb.y, margin, 'compare variable assignment 2');
    }

    function test_calculated_height() {
        sb.text = "";
        compare(sb.height, 64, 'verify height with no text');
        sb.text = "s";
        compare(sb.height, 64, 'verify height with 1 line of text');
        sb.text = "ss";
        compare(sb.height, 80, 'verify height with 2 lines of text');
        sb.text = "sss";
        compare(sb.height, 106, 'verify height with 3 lines of text');
    }

    function cleanupTestCase() {
        sb.iconSource = '';
        sb.text = '';
    }
}
