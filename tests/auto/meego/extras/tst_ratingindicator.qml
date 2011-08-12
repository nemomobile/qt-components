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
    name: "RatingIndicator"

    //rating indicator with vote count
    RatingIndicator {
        id: indicator
        ratingValue: 2
        maximumValue: 5
        count: 3
    }

    function test_a_increase_rating() {
        indicator.ratingValue+=2;
        compare(indicator.ratingValue, 4);
        indicator.ratingValue+=10;
        compare(indicator.ratingValue, 5); // value should be set to maximumValue instead
    }
    function test_b_decrease_rating() {
        indicator.ratingValue-=2;
        compare(indicator.ratingValue, 3);
        indicator.ratingValue-=10;
        compare(indicator.ratingValue, 0); // value should be set to 0 instead
    }
    function test_c_increase_votes() {
        indicator.count+=2;
        compare(indicator.count, 5);
        indicator.count+=1000000000;
        compare(indicator.count, 1000000005); //check really large count number
    }
    function test_d_decrease_votes() {
        indicator.count-=1000000000;
        compare(indicator.count, 5);
        indicator.count-=10;
        compare(indicator.count, -5); // no boundary check for count
    }
    function test_e_inverted_type() {
        var c = indicator.children[1].color;
        indicator.inverted = true;
        verify(c != indicator.children[1].color,"colors should be different");
    }
    function test_f_normal_type() {
        var c = indicator.children[1].color;
        indicator.inverted = false;
        verify(c != indicator.children[1].color,"colors should be different");
    }
}
