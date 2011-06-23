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

#include "qglnamespace.h"

QT_BEGIN_NAMESPACE

/*!
    \namespace QGL

    \brief The QGL namespace contains miscellaneous identifiers
    used throughout the Qt/3D module.
*/

/*!
    \enum QGL::VertexAttribute
    \since 4.8
    This enum defines the type of vertex attribute to set on an effect with QGLPainter::setVertexAttribute()

    \value Position The primary position of the vertex.
    \value Normal The normal at each vertex.
    \value Color The color at each vertex.
    \value TextureCoord0 The texture co-ordinate at each vertex for texture unit 0.
    \value TextureCoord1 The texture co-ordinate at each vertex for texture unit 1.
    \value TextureCoord2 The texture co-ordinate at each vertex for texture unit 2.
    \value CustomVertex0 First custom vertex attribute.  Custom attributes
           can be used for any purpose: texture co-ordinates, colors,
           or other values of interest to shader programs.
    \value CustomVertex1 Second custom vertex attribute.
    \value UserVertex First user-assigned vertex attribute.  Additional
           attributes can be assigned as UserVertex, UserVertex + 1, etc.
           Note that on OpenGL/ES 2.0 systems, usually the maximum
           number of vertex attributes is 8, which means that user-assigned
           vertex attributes will be out of range.
*/

/*!
    \enum QGL::Face
    \since 4.8
    This enum defines the faces to apply an operation to.

    \value FrontFaces Apply the operation to front faces only.
    \value BackFaces Apply the operation to back faces only.
    \value AllFaces Apply the operation to both front and back faces.
*/

/*!
    \enum QGL::CullFace
    \since 4.8
    This enum defines the face culling mode to use with QGLPainter::setCullFaces().

    \value CullDisabled Face culling is disabled.
    \value CullFrontFaces Cull front faces, but not back faces.
    \value CullBackFaces Cull back faces, but not front faces.
    \value CullAllFaces Cull both front and back faces.
    \value CullClockwise Flag that indicates that clockwise faces should
           be treated as front-facing instead of the default whereby
           counter-clockwise faces are treated as front-facing.
*/

/*!
    \enum QGL::DrawingMode
    \since 4.8
    This enum defines the type of OpenGL primitive to render with QGLPainter::draw().

    \value Points Draws a point at each of the specified vertices.
    \value Lines Draws a series of unconnected line segments, using two
           vertices for each line.
    \value LineLoop series of connected line segments, from the
           first to the last vertex, and then connecting the last
           vertex back to the first vertex.
    \value LineStrip Draws a series of connected line segments, from the
           first to the last vertex.
    \value Triangles Draws a series of triangles using three vertices from
           the enabled vertex arrays for each triangle.
    \value TriangleStrip Draws a series of triangles in a strip, starting
           with the first three vertices and then one extra vertex for each
           additional triangle.
    \value TriangleFan Draws a series of triangles that fan out around the
           first vertex in the enabled vertex arrays, starting with the
           first three vertices and then one extra vertex for each additional
           triangle.
    \value LinesAdjacency Draws a series of unconnected lines, using
           two vertices for each line to define the positions, and an
           additional vertices per line to define adjacent points.
           This drawing mode is only supported on OpenGL systems that
           have geometry shaders.
    \value LineStripAdjacency Draws a series of connected line segments,
           from the second to the second last vertex.  The first and last
           vertices define adjacent points.  This drawing mode is only
           supported on OpenGL systems that have geometry shaders.
    \value TrianglesAdjacency Draws a series of triangles using three
           vertices from the enabled vertex arrays for each triangle.
           An additional three vertices per triangle are supplied to
           define adjacent points.  This drawing mode is only supported
           on OpenGL systems that have geometry shaders.
    \value TriangleStripAdjacency Draws a series of triangles in a strip,
           with additional vertices for points adjacent to the strip.
           This drawing mode is only supported on OpenGL systems that
           have geometry shaders.
*/

/*!
    \enum QGL::StandardEffect
    \since 4.8
    This enum defines a standard drawing effect for use with QGLPainter.

    \value FlatColor Single flat color specified by QGLPainter::setColor()
           with no lighting.
    \value FlatPerVertexColor Per-vertex flat colors specified by a
           QGL::Color vertex array with no lighting.
    \value FlatReplaceQGLTexture2D Map a texture across the fragments with
           no lighting.  The final fragment color is replaced directly
           with the texture.  The texture is sourced from texture unit 0.
    \value FlatDecalQGLTexture2D Map a texture across the fragments, combined
           with QGLPainter::color(), and no lighting.  The texture is sourced
           from texture unit 0.
    \value LitMaterial Material colors specified by
           QGLPainter::setFaceMaterial() with lighting enabled.  It is
           assumed that per-vertex normals are provided.  Under OpenGL/ES 2.0
           only one light is supported, with single-sided materials,
           and no attenuation.
    \value LitDecalQGLTexture2D Map a texture across the fragments, combined
           with the material color specified by QGLPainter::setFaceMaterial(),
           and lighting using the GL_DECAL combination rule.  The texture is
           sourced from texture unit 0.  It is assumed that per-vertex
           normals are provided.  Under OpenGL/ES 2.0 only one light is
           supported, with single-sided materials, and no attenuation.
    \value LitModulateQGLTexture2D Map a texture across the fragments, combined
           with the material color specified by QGLPainter::setFaceMaterial(),
           and lighting using the GL_MODULATE combination rule.  The texture
           is sourced from texture unit 0.  It is assumed that per-vertex
           normals are provided.  Under OpenGL/ES 2.0 only one light is
           supported, with single-sided materials, and no attenuation.
*/

/*!
    \enum QGL::TextureWrap
    \since 4.8
    This enum defines the wrapping mode for texture co-ordinates.

    \value Repeat Ignore the integer part of the texture co-ordinate and
        create a repeating pattern using the texture.
    \value Clamp Clamps the texture co-ordinate to the range 0 to 1.
        This value is not supported in OpenGL/ES, where it will be
        treated the same as ClampToEdge.
    \value ClampToBorder Clamps the texture co-ordinate to the range
        (-1 / 2N) to (1 + 1 / 2N) where N is the size of the texture in
        the direction of clamping.  This value is supported in OpenGL 1.3
        and higher.  In OpenGL versions less than 1.3, this value will
        be treated the same as Clamp.  In OpenGL/ES, this value will be
        treated the same as ClampToEdge.
    \value ClampToEdge Clamps the texture co-ordinate to the range
        (1 / 2N) to (1 - 1 / 2N) where N is the size of the texture in
        the direction of clamping.  This value is supported in OpenGL 1.2
        and higher, and in all versions of OpenGL/ES.  In OpenGL versions
        less than 1.2, this value will be treated the same as Clamp.
    \value MirroredRepeat If the integer part of the texture co-ordinate
        is even, then use the fractional part.  If the integer part of
        the texture co-ordinate is odd, then use 1 - the fractional part.
        This creates an alternating repeating pattern from the texture.
        This value is only supported in OpenGL 1.4 and higher, or
        OpenGL/ES 2.0 and higher.  On other versions, MirroredRepeat
        will be treated the same as Repeat.
*/

/*!
    \enum QGL::ClearBuffer
    \since 4.8
    This enum defines a buffer to be cleared with glClear().

    \value NoClearBuffers No buffers should be cleared.
    \value ClearDepthBuffer Clears the depth buffer.
    \value ClearStencilBuffer Clears the stencil buffer.
    \value ClearColorBuffer Clears the color buffer.
*/

/*!
    \enum QGL::Eye
    \since 4.8
    This enum defines the eye that is being rendered by a QGLPainter
    when stereo rendering is in effect.

    \value NoEye Do not perform an eye adjustment on the camera because
        stereo rendering is not in effect.
    \value LeftEye Render the scene from the perspective of the left eye.
    \value RightEye Render the scene from the perspective of the right eye.
*/

/*!
    \enum QGL::Smoothing
    \since 4.8
    \relates QGLDisplayList

    This enum defines vertex smoothing treatments.
    \value NoSmoothing No smoothing processing is performed.
    \value Smooth Lighting normals averaged for each face for a smooth appearance.
    \value Faceted Lighting normals separate for each face for a faceted appearance.
*/

QT_END_NAMESPACE
