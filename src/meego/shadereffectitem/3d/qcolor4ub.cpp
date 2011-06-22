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

#include "qcolor4ub.h"
#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

/*!
    \class QColor4ub
    \brief The QColor4ub class represents a color by four unsigned byte components.
    \since 4.8
    \ingroup qt3d
    \ingroup qt3d::enablers

    OpenGL applications commonly use four unsigned byte values to compactly
    represent a color value.  QColor4ub provides a convenience
    class for manipulating such compact color values.

    An alternative is to represent a color value as four floating-point
    values between 0.0 and 1.0.  The QVector4D class can be used for
    that purpose if required.
*/

/*!
    \fn QColor4ub::QColor4ub()

    Constructs a four-byte default color value of (0, 0, 0, 255).
*/

/*!
    \fn QColor4ub::QColor4ub(int red, int green, int blue, int alpha)

    Constructs a four-byte color value with the components \a red,
    \a green, \a blue, and \a alpha.
*/

/*!
    \fn QColor4ub::QColor4ub(const QColor& color)

    Constructs a four-byte color value from \a color.
*/

/*!
    \fn QColor4ub::QColor4ub(Qt::GlobalColor color)

    Constructs a four-byte color value from \a color.
*/

/*!
    \fn QColor4ub::QColor4ub(QRgb rgba)

    Constructs a four-byte color value from the red, green, blue, and
    alpha components of \a rgba.
*/

/*!
    \fn QColor4ub& QColor4ub::operator=(const QColor& color)

    Copies the red, green, blue, and alpha components of \a color
    into this object.
*/

/*!
    \fn QColor4ub& QColor4ub::operator=(Qt::GlobalColor color)

    Copies the red, green, blue, and alpha components of the
    specified global \a color name into this object.
*/

/*!
    \fn int QColor4ub::red() const

    Returns the red component of this color, between 0 and 255.

    \sa green(), blue(), alpha(), setRed(), redF()
*/

/*!
    \fn int QColor4ub::green() const

    Returns the green component of this color, between 0 and 255.

    \sa red(), blue(), alpha(), setGreen(), greenF()
*/

/*!
    \fn int QColor4ub::blue() const

    Returns the blue component of this color, between 0 and 255.

    \sa red(), green(), alpha(), setBlue(), blueF()
*/

/*!
    \fn int QColor4ub::alpha() const

    Returns the alpha component of this color, between 0 and 255.

    \sa red(), green(), blue(), setAlpha(), alphaF()
*/

/*!
    \fn void QColor4ub::setRed(int value)

    Sets the red component of this color to \a value, between 0 and 255.

    \sa setGreen(), setBlue(), setAlpha(), red(), setRedF()
*/

/*!
    \fn void QColor4ub::setGreen(int value)

    Sets the green component of this color to \a value, between 0 and 255.

    \sa setRed(), setBlue(), setAlpha(), green(), setGreenF()
*/

/*!
    \fn void QColor4ub::setBlue(int value)

    Sets the blue component of this color to \a value, between 0 and 255.

    \sa setRed(), setGreen(), setAlpha(), blue(), setBlueF()
*/

/*!
    \fn void QColor4ub::setAlpha(int value)

    Sets the alpha component of this color to \a value, between 0 and 255.

    \sa setRed(), setGreen(), setBlue(), alpha(), setAlphaF()
*/

/*!
    \fn qreal QColor4ub::redF() const { return m_red / 255.0f; }

    Returns the red component of this color as a floating-point
    value between 0 and 1.

    \sa greenF(), blueF(), alphaF(), setRedF(), red()
*/

/*!
    \fn qreal QColor4ub::greenF() const { return m_green / 255.0f; }

    Returns the green component of this color as a floating-point
    value between 0 and 1.

    \sa redF(), blueF(), alphaF(), setGreenF(), green()
*/

/*!
    \fn qreal QColor4ub::blueF() const { return m_blue / 255.0f; }

    Returns the blue component of this color as a floating-point
    value between 0 and 1.

    \sa redF(), greenF(), alphaF(), setBlueF(), blue()
*/

/*!
    \fn qreal QColor4ub::alphaF() const { return m_alpha / 255.0f; }

    Returns the alpha component of this color as a floating-point
    value between 0 and 1.

    \sa redF(), greenF(), blueF(), setAlphaF(), alpha()
*/

/*!
    \fn void QColor4ub::setRedF(qreal value)

    Sets the red component of this color to a floating-point \a value,
    between 0 and 1.

    \sa setGreenF(), setBlueF(), setAlphaF(), redF(), setRed()
*/

/*!
    \fn void QColor4ub::setGreenF(qreal value)

    Sets the green component of this color to a floating-point \a value,
    between 0 and 1.

    \sa setRedF(), setBlueF(), setAlphaF(), greenF(), setGreen()
*/

/*!
    \fn void QColor4ub::setBlueF(qreal value)

    Sets the blue component of this color to a floating-point \a value,
    between 0 and 1.

    \sa setRedF(), setGreenF(), setAlphaF(), blueF(), setBlue()
*/

/*!
    \fn void QColor4ub::setAlphaF(qreal value)

    Sets the alpha component of this color to a floating-point \a value,
    between 0 and 1.

    \sa setRedF(), setGreenF(), setBlueF(), alphaF(), setAlpha()
*/

/*!
    \fn void QColor4ub::setRgb(int red, int green, int blue, int alpha)

    Sets the components of this color to \a red, \a green, \a blue,
    and \a alpha.  Each component is between 0 and 255.

    \sa setRgbF(), fromRgb()
*/

/*!
    \fn void QColor4ub::setRgbF(qreal red, qreal green, qreal blue, qreal alpha)

    Sets the components of this color to \a red, \a green, \a blue,
    and \a alpha.  Each component is a floating-point value between 0 and 1.

    \sa setRgb(), fromRgbF()
*/

/*!
    \fn QColor4ub QColor4ub::fromRgb(int red, int green, int blue, int alpha)

    Returns a QColor4ub with the components \a red, \a green, \a blue,
    and \a alpha.  Each component is between 0 and 255.

    \sa fromRgbF(), setRgb()
*/

/*!
    \fn QColor4ub QColor4ub::fromRgbF(qreal red, qreal green, qreal blue, qreal alpha)

    Returns a QColor4ub with the components \a red, \a green, \a blue,
    and \a alpha.  Each component is a floating-point value between 0 and 1.

    \sa fromRgb(), setRgbF()
*/

/*!
    \fn QColor4ub QColor4ub::fromRaw(const uchar *data)

    Returns a QColor4ub with components from the first four elements
    in \a data.  The \a data parameter must contain at least four
    elements and not be null.
*/

/*!
    \fn QColor QColor4ub::toColor() const

    Returns this color as a QColor.
*/

/*!
    \fn bool QColor4ub::operator==(const QColor4ub& other) const

    Returns true if this color is the same as \a other; false otherwise.
*/

/*!
    \fn bool QColor4ub::operator!=(const QColor4ub& other) const

    Returns true if this color is not the same as \a other; false otherwise.
*/

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<(QDebug dbg, const QColor4ub &color)
{
    dbg.nospace() << "QColor4ub("
        << color.redF() << ", " << color.greenF() << ", "
        << color.blueF() << ", " << color.alphaF() << ')';
    return dbg.space();
}

#endif

QT_END_NAMESPACE
