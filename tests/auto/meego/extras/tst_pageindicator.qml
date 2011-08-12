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
    name: "PageIndicator"

    //rating indicator with vote count
    PageIndicator {
        id: indicator
        currentPage: 1
        totalPages: 3
    }

    // for testing visual change specifically
    // eliminate the elements added by repeaters
    PageIndicator {
        id: indicator2
        currentPage: 1
        totalPages: 1
    }

    function init() {
        indicator.currentPage = 1;
        indicator.totalPages = 3;
    }

    function test_property_currentPage() {
        indicator.currentPage++;
        compare(indicator.currentPage, 2, 'currentPage should be 2');
        indicator.currentPage += 10;
        compare(indicator.currentPage, indicator.totalPages, 'currentPage should be equal to the total pages'); // value should be set to total instead

        // decrease
        indicator.currentPage--;
        compare(indicator.currentPage, 2, 'currentPage should be 2 again');
        indicator.currentPage -= 10;
        compare(indicator.currentPage, 1, 'currentPage should be 1'); // value should be set to 1 instead
    }

    function test_property_totalPages() {
        var w = indicator.width;
        indicator.totalPages++;
        compare(indicator.totalPages, 4, 'totalPages should be 4');
        compare(indicator.width > w, true, 'the width should be increased');

        // decrease
        w = indicator.width;
        indicator.totalPages--;
        compare(indicator.totalPages, 3, 'totalPages should be 3');
        compare(indicator.width < w, true, 'the width should be decreased');
        indicator.totalPages-=10;
        compare(indicator.totalPages, 1, 'totalPages should be 1'); // total should be set to 1 instead
    }

    function test_property_inverted() {
        // row & repeater counts as visual children
        var s = indicator2.children[0].children[1].source;
        indicator2.inverted = true;
        verify(s != indicator2.children[0].children[1].source, 'image source should be different (inverted)');

        var s = indicator2.children[0].children[1].source;
        indicator2.inverted = false;
        verify(s != indicator2.children[0].children[1].source, 'image source should be different (normal)');
    }
}
