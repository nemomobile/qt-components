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

function openDatabase() {
    return openDatabaseSync("musicplayer", "1.0", "Settings", 100000)
}

function initialize() {
    var db = openDatabase()
    db.transaction(
        function(tx) {
            tx.executeSql("CREATE TABLE IF NOT EXISTS settings(setting TEXT UNIQUE, value TEXT)")
            tx.executeSql("CREATE TABLE IF NOT EXISTS playlist(source TEXT, title TEXT, artist TEXT, time TEXT)")
        }
    )
}

function setSetting(setting, value) {
    var db = openDatabase()
    var res = "Error"
    db.transaction(
        function(tx) {
            var rs = tx.executeSql("INSERT OR REPLACE INTO settings VALUES (?,?);", [setting,value])
            if (rs.rowsAffected > 0)
                res = "OK"
        }
    )
    return res
}

function getSetting(setting) {
    var db = openDatabase()
    var res = "Unknown"
    db.transaction(
        function(tx) {
            var rs = tx.executeSql("SELECT value FROM settings WHERE setting=?;", [setting])
            if (rs.rows.length > 0)
                res = rs.rows.item(0).value
        }
    )
    return res
}

function getPlaylist(playlist) {
    var db = openDatabase()
    var res = "Unknown"
    db.transaction(
        function(tx) {
            var rs = tx.executeSql("SELECT * FROM playlist;")
            if (rs.rows.length > 0) {
                for (var i = 0; i < rs.rows.length; i++) {
                    playlist.append({"source": rs.rows.item(i).source,
                                    "title": rs.rows.item(i).title,
                                    "artist": rs.rows.item(i).artist,
                                    "time": rs.rows.item(i).time,
                                    "selected": false})
                }
                res = "OK"
            }
        }
    )
    return res
}

function setPlaylist(playlist) {
    var db = openDatabase()
    var res = "Error"
    db.transaction(
        function(tx) {
            var rs = tx.executeSql("DELETE FROM playlist;")
            var count = 0
            for (var i = 0; i < playlist.count; i++) {
                rs = tx.executeSql("INSERT INTO playlist VALUES (?,?,?,?);",
                    [playlist.get(i).source, playlist.get(i).title, playlist.get(i).artist, playlist.get(i).time])
                count += rs.rowsAffected
            }

            if (count > 0)
                res = "OK"
        }
    )
    return res
}
