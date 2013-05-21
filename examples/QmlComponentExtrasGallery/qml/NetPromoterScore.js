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

.pragma library                         // Singleton

// Internal helper function
function _optionString(options) {
    var keys = [];
    for (var key in options) {
        keys.push(key);
    }
    //keys.sort();

    var opts = [];
    for (var i = 0; i < keys.length; i++) {
        var key = keys[i];
        var val = options[key];
        opts.push(key + "=" + encodeURIComponent(val))
    }

    return (opts.length)? opts.join("&") : "";
}

// Sends report to NPS server
// env: "dev", "qa", or "prd" NPS server
// opts: report parameters
// callback: callback function on succuss
// errback: callback function on error
function postNpsReport(env, opts, callback, errback) {
    var npsUri = {
        dev: "https://cpqit.nokia.com/api.php?",
        qa: "https://cpqqa.nokia.com/api.php?",
        prd:"https://cpq.nokia.com/api.php?"
    }

    var uri = npsUri[env]|| npsUri.qa;

    var mark = (new Date()).valueOf();
    var options = opts || {};
    var path = uri + _optionString(options);


    console.log("NPS: " + path);
    var xhr = new XMLHttpRequest();
    xhr.open('POST', path, true);
    var headers = options.headers || {};
    for (var key in headers) {
        var value = headers[key];
        if (value) xhr.setRequestHeader(key, value);
    }

    xhr.onreadystatechange = function() {
        if (xhr.readyState === 4) {
            var now = (new Date()).valueOf();
            console.log("NPS: run-time=" + (now-mark) + "ms");
            var status = xhr.status;
            var resp = xhr.responseText;
            console.log("NPS: response: " + resp)
            if (status == 200) {
                if(callback) callback(resp);
            } else if (errback) {
                console.log("error status=" + status + "-" + xhr.statusText);
                if(errback) errback(status);
            }
        }
    };
    xhr.send(null);
}

