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

import QtQuick 1.0
import QtMultimediaKit 1.1

Item {
    id: player

    property bool playing: false
    property int index: -1
    property alias playlistModel: playlist
    property alias count: playlist.count
    property string title: audio.metaData.title != undefined && audio.source != "" ? audio.metaData.title : getFileFromPath(audio.source.toString())
    property int duration: audio.source != "" ? audio.duration : 0
    property string durationTime: audio.source != "" ? getTimeFromMSec(audio.duration) : ""
    property string artist: audio.metaData.albumArtist != undefined && audio.source != "" ? audio.metaData.albumArtist : ""
    property bool seekable: audio.seekable
    property alias position: audio.position
    property string positionTime: getTimeFromMSec(position)
    property alias volume: audio.volume
    property bool repeat: false
    property bool shuffle: false
    property string error: audio.errorString
    property alias playlist:  playlist

    signal playlistLoaded

    function play() {
        audio.play()
        playing = true
    }

    function pause() {
        audio.pause()
        playing = false
    }

    function stop() {
        audio.stop()
        playing = false
    }

    function previous() {
        var i = shuffle ? (Math.random() * (count - 1)) : (index - 1)
        if (i < 0)
            i = repeat ? (count - 1) : 0

        index = i
        refreshSong()
    }

    function next() {
        var i = shuffle ? (Math.random() * (count - 1)) : (index + 1)
        if (i > count - 1) {
            if (repeat) {
                i = 0
            } else {
                i = count - 1
                playing = false
            }
        }

        index = i
        refreshSong()
    }

    function addSong(file, title, artist, duration) {
        playlist.append({"source": file,
                        "title": title,
                        "artist": artist,
                        "time": getTimeFromMSec(duration),
                        "selected": false})

        if (playlist.count == 1) {
            index = 0
            refreshSong()
            play()
        }
    }

    function remove(i) {
        playlist.remove(i)

        if (playlist.count == 0) {
                stop()

            audio.source = ""
            return
        }

        if (i < index) {
            index--
        } else if (i == index) {
            if (index > count - 1)
                index = count - 1
        }
    }

    function refreshSong() {
        var wasPlaying = playing
            stop()

        if (index >= 0 && playlist.count > 0) {
            audio.source = playlist.get(index).source
            if (wasPlaying)
                play()
        }
    }

    function getFileFromPath(path) {
        return path.substring(path.lastIndexOf(path.charAt(path.indexOf(":") + 1)) + 1)
    }

    function getTimeFromMSec(msec) {
        if (msec <= 0 || msec == undefined) {
            return ""

        } else {
            var sec = "" + Math.floor(msec / 1000) % 60
            if (sec.length == 1)
                sec = "0" + sec

            var hour = Math.floor(msec / 3600000)
            if (hour < 1) {
                return Math.floor(msec / 60000) + ":" + sec
            } else {
                var min = "" + Math.floor(msec / 60000) % 60
                if (min.length == 1)
                    min = "0" + min

                return hour + ":" + min + ":" + sec
            }
        }
    }

    function updatePlaylistData() {
        playlist.setProperty(index, "time", durationTime)
        playlist.setProperty(index, "artist", artist)
        playlist.setProperty(index, "title", title)
    }

    onDurationTimeChanged: playlist.setProperty(index, "time", durationTime)
    onTitleChanged: playlist.setProperty(index, "title", title)
    onArtistChanged: playlist.setProperty(index, "artist", artist)

    ListModel {
        id: playlist
    }

    Audio {
        id: audio

        volume: 0.5

        onStatusChanged: {
            if (status == Audio.EndOfMedia) {
                next()
            }
        }

        onPlayingChanged: updatePlaylistData()
    }
}
