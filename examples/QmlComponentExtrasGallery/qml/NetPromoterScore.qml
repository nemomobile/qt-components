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
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import "NetPromoterScore.js" as NPS

Page {
    id: container
    anchors.margins: UiConstants.DefaultMargin
    tools: commonTools

    function _submitReport() {

        // Build report parameters
        var params = {};
        params.projid = '2016';
        params.version = 'manual';
        params.pageurl = "Qml";
        params.rec = npsForm.scale;
        if (npsForm.comments) {
            params.fb_text = npsForm.comments;
        }
        params.device = "RM-123";
        params.hw = "N900";
        params.sw = "PR1.1";
        params.imsi = "123456789012"
        params.nokia_account = "oviaccount";
        if (npsForm.useEmail) {
            params.fb_email = "email@ovi.com";
        } else if (npsForm.emailAddress) {
            params.fb_email = npsForm.emailAddress;
        }

        NPS.postNpsReport("qa", params,
                    function() {
                        console.log("NPS report sent ok.")
                    },
                    function(reason) {
                        console.log("Failed to send NPS report.")
                    });
    }

    Flickable {
        width: rect.width
        height: parent.height
        contentWidth: rect.width
        contentHeight: rect.height

        // To make scrolling the feedback text smoother when it has focus
        pressDelay: 200

        Rectangle {
            id: rect
            width: 480
            height: childrenRect.height

            NetPromoterScore {
                id: npsForm

                width: parent.width

                uiString:  {
                    return {
                            header: "Tell us what you think",
                            question: "How likely are you to recommend Qt-Component to a friend or a colleague?",
                            notLikely: "Not at all\nlikely",
                            extremelyLikely: 'Extremely\nlikely',
                            selectScore: 'You must select a rating',
                            commentsInputLabel: 'Please tell us why you gave this score (optional)',
                            contactAgreement: 'You can contact me for details using my Nokia Account e-mail (test@nokia.com)',
                            emailInputLabel: 'You can contact me for details (Optional)',
                            emailPlaceholderText: 'Your email address',
                            emailSipActionKeyLabel: 'Done',
                            invalidEmail: 'Invalid email address',
                            legalText: 'Your information will be treated according to Nokia privacy policy',
                            submit: 'Submit',
                    }
                }

                isSignedIn: false

                onSubmit: {
                    console.log("Score=" + score + ", comments=" + comments +
                                ", email=" + emailAddress + ", useEmail=" + useEmail)

                    _submitReport();
                }
            }
        }
    }
}
