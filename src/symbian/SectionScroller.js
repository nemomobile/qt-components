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

var _sectionData = [];
var _sections = [];

function initSectionData(list) {
    if (!list || !list.model) return;
    _sectionData = [];
    _sections = [];
    var current = "";
    var prop = list.section.property;

    for (var i = 0, count = list.model.count; i < count; i++) {
        var item = list.model.get(i);
        if (item[prop] !== current) {
            current = item[prop];
            _sections.push(current);
            _sectionData.push({ index: i, header: current });
        }
    }
}

function sectionPositionString(name) {
    var val = _sections.indexOf(name);
    return val === 0 ? "first" :
           val === _sections.length - 1 ? "last" : false;
}

function sectionAt(pos) {
    return _sections[pos] ? _sections[pos] : "";
}

function relativeSections(current) {
    var val = _sections.indexOf(current);
    var sect = [];
    var sl = _sections.length;

    val = val < 1 ? 1 : val >= sl - 1 ? sl - 2 : val;
    sect = [sectionAt(val - 1), sectionAt(val), sectionAt(val + 1)];

    return sect;
}

function closestSection(pos, down) {
    var tmp = (_sections.length) * pos;
    var val = Math.ceil(tmp) // TODO: better algorithm
    val = val < 2 ? 1 : val;
    return _sections[val-1];
}

function nextSection(current) {
    var val = _sections.indexOf(current);
    return (val > -1 ?
                _sections[(val < _sections.length - 1 ? val + 1 : val)] :
                _sections[0])
            || "";
}

function previousSection(current) {
    var val = _sections.indexOf(current);
    return (val > -1 ?
                _sections[(val > 0 ? val - 1 : val)] :
                _sections[0])
            || "";
}

function indexOf(sectionName) {
    var val = _sectionData[_sections.indexOf(sectionName)].index;
    return val === 0 || val > 0 ? val : -1;
}
