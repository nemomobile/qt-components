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
