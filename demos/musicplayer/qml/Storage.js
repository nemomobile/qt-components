/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
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
