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

#ifndef QGLNAMESPACE_H
#define QGLNAMESPACE_H

#include "qt3dglobal.h"

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

namespace QGL
{
    enum VertexAttribute {
        Position,
        Normal,
        Color,
        TextureCoord0,
        TextureCoord1,
        TextureCoord2,
        CustomVertex0,
        CustomVertex1,
        UserVertex
    };

    enum Face
    {
        FrontFaces                  = 0x0404, // GL_FRONT
        BackFaces                   = 0x0405, // GL_BACK
        AllFaces                    = 0x0408  // GL_FRONT_AND_BACK
    };

    enum CullFace
    {
        CullDisabled                = 0,
        CullFrontFaces              = 0x0404, // GL_FRONT
        CullBackFaces               = 0x0405, // GL_BACK
        CullAllFaces                = 0x0408, // GL_FRONT_AND_BACK
        CullClockwise               = 0x10000
    };
    Q_DECLARE_FLAGS(CullFaces, CullFace)

    enum DrawingMode
    {
        Points                      = 0x0000, // GL_POINTS
        Lines                       = 0x0001, // GL_LINES
        LineLoop                    = 0x0002, // GL_LINE_LOOP
        LineStrip                   = 0x0003, // GL_LINE_STRIP
        Triangles                   = 0x0004, // GL_TRIANGLES
        TriangleStrip               = 0x0005, // GL_TRIANGLE_STRIP
        TriangleFan                 = 0x0006, // GL_TRIANGLE_FAN
        LinesAdjacency              = 0x000A, // GL_LINES_ADJACENCY
        LineStripAdjacency          = 0x000B, // GL_LINE_STRIP_ADJACENCY
        TrianglesAdjacency          = 0x000C, // GL_TRIANGLES_ADJACENCY
        TriangleStripAdjacency      = 0x000D  // GL_TRIANGLE_STRIP_ADJACENCY
    };

    enum StandardEffect
    {
        FlatColor,
        FlatPerVertexColor,
        FlatReplaceQGLTexture2D,
        FlatDecalQGLTexture2D,
        LitMaterial,
        LitDecalQGLTexture2D,
        LitModulateQGLTexture2D
    };

    enum TextureWrap
    {
        Repeat                      = 0x2901,   // GL_REPEAT
        Clamp                       = 0x2900,   // GL_CLAMP
        ClampToBorder               = 0x812D,   // GL_CLAMP_TO_BORDER
        ClampToEdge                 = 0x812F,   // GL_CLAMP_TO_EDGE
        MirroredRepeat              = 0x8370    // GL_MIRRORED_REPEAT
    };

    enum ClearBuffer
    {
        NoClearBuffers              = 0x0000,
        ClearDepthBuffer            = 0x0100,   // GL_DEPTH_BUFFER_BIT
        ClearStencilBuffer          = 0x0400,   // GL_STENCIL_BUFFER_BIT
        ClearColorBuffer            = 0x4000    // GL_COLOR_BUFFER_BIT
    };
    Q_DECLARE_FLAGS(ClearBuffers, ClearBuffer)

    enum Eye
    {
        NoEye,
        LeftEye,
        RightEye
    };

    enum Smoothing
    {
        NoSmoothing,
        Smooth,
        Faceted
    };
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QGL::CullFaces)
Q_DECLARE_OPERATORS_FOR_FLAGS(QGL::ClearBuffers)

QT_END_NAMESPACE

QT_END_HEADER

#endif
