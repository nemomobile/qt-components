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

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <qdeclarativecontext.h>
#include <QDeclarativeView>
#include <QTextOption>
#include <QFontDatabase>
#include <QDeclarativeItem>
#include <QDeclarativeExpression>
#include <QClipboard>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentstextarea : public QObject

{
    Q_OBJECT
public:
    tst_quickcomponentstextarea();

private slots:
    void initTestCase();
    void font();
    void cursorPosition();
    void horizontalAlignment();
    void verticalAlignment();
    void readOnly();
    void selectedText();
    void selectionEnd();
    void selectionStart();
    void text();
    void textFormat();
    void copypaste();
    void cutpaste();
    void positionAt();
    void positionToRectangle();
    void wrapMode();
    void inputMethodHints();
    void errorHighlight();
    void softwareInputPanel();

private:
    // our internal objects
    QObject *componentObject;
    QGraphicsObject *root;
    QGraphicsObject *textEdit;

    // font helpers
    QFont mfont;
    QString family;
    int fontSize;
};

tst_quickcomponentstextarea::tst_quickcomponentstextarea()
    : componentObject(NULL)
    , root(NULL)
    , textEdit(NULL)
{
}

void tst_quickcomponentstextarea::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentstextarea.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));

    // select a font from the database
    QFontDatabase database;
    family = database.families().at(0);
    fontSize = database.smoothSizes(family, database.styles(family).at(0)).at(0);

    // create the font based on the one found in the system and set it
    mfont = QFont(family, fontSize);
    componentObject->setProperty("font", mfont);

    // we need to find the child that is a QDeclarativeTextInput in order to properly
    // map the position so this test doesn't fail when there are margins.
    // this is a safe assumption for symbian and meego components as these tests
    // aims to test the features of those two platforms components.
    root = qobject_cast<QGraphicsObject*>(componentObject);
#ifdef Q_COMPONENTS_SYMBIAN
    // Symbian style has two TextEdit instances. Find with objectName.
    textEdit = componentObject->findChild<QGraphicsObject*>("textEdit");
#else
    // Assuming TextEdit is a direct child of TextArea.
    const QObjectList list = componentObject->children();
    foreach (QObject *obj, list) {
        if (obj->metaObject()->className() == QString("QDeclarativeTextEdit")) {
            textEdit = qobject_cast<QGraphicsObject*>(obj);
            break;
        }
    }
#endif

    QVERIFY(root);
    QVERIFY(textEdit);
}

void tst_quickcomponentstextarea::font()
{
    QFont temp;
    QVERIFY(componentObject->setProperty("font", mfont));
    temp = componentObject->property("font").value<QFont>();
    QCOMPARE(temp, mfont);
}

void tst_quickcomponentstextarea::cursorPosition()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("cursorPosition", 0));
    QCOMPARE(componentObject->property("cursorPosition").toInt(), 0);
}

void tst_quickcomponentstextarea::horizontalAlignment()
{
    // test setting/getting the property
    QVERIFY( componentObject->setProperty("horizontalAlignment", 0) );
    QCOMPARE( componentObject->property("horizontalAlignment").toInt(), 0 );
}

void tst_quickcomponentstextarea::verticalAlignment()
{
    // test setting/getting the property
    QVERIFY( componentObject->setProperty("verticalAlignment", 0) );
    QCOMPARE( componentObject->property("verticalAlignment").toInt(), 0 );
}

void tst_quickcomponentstextarea::readOnly()
{
    QVERIFY(componentObject->setProperty("readOnly", true));
    QVERIFY(componentObject->setProperty("text", "I just changed the text"));
    QEXPECT_FAIL("", "Not yet guarded by readOnly property, http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-318", Continue);
    QVERIFY(componentObject->property("text").toString() != QString("I just changed the text"));

    // if readOnly == true, we should not be able to cut or paste
    // set the text to something known, select and cut it
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "cut"), "Could not cut the text");

    // check if the text area was cleared
    // ###: Change the string being compared here when the test above is fixed (readOnly guard)
    QCOMPARE(componentObject->property("text").toString(), QString("I just changed the text"));

    // we shouldn't be able to paste stuff on it too
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "copy"), "Could not copy the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "paste"), "Could not paste the text");

    // the text should remain the same as readOnly == true
    // ###: Change the string being compared here when the test above is fixed (readOnly guard)
    QCOMPARE(componentObject->property("text").toString(), QString("I just changed the text"));

    // reset readonly so it doesn't affect the next tests
    componentObject->setProperty("readOnly", false);
}

void tst_quickcomponentstextarea::selectedText()
{
    componentObject->setProperty("text", "Good morning");

    // test select(start, end)
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 0), Q_ARG(QVariant, 4)),
             "Could not select from 0-4");
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good") );

    // test selectAll()
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good morning") );

    // test selectWord (it will select the word closest to the cursor)
    componentObject->setProperty("cursorPosition", 0);
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectWord"), "Could not select one word");
    QCOMPARE(componentObject->property("selectedText").toString(), QString("Good") );
}

void tst_quickcomponentstextarea::selectionStart()
{
    componentObject->setProperty("text", "Good morning");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 5), Q_ARG(QVariant, 11)),
             "Could not select from 5-11");
    QCOMPARE(componentObject->property("selectionStart").toInt(), 5);
}

void tst_quickcomponentstextarea::selectionEnd()
{
    componentObject->setProperty("text", "Good morning");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "select", Q_ARG(QVariant, 5), Q_ARG(QVariant, 11)),
             "Could not select from 5-11");
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 11);
}

void tst_quickcomponentstextarea::text()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("text", "Hello World"));
    QCOMPARE(componentObject->property("text").toString(), QString("Hello World"));
}

void tst_quickcomponentstextarea::textFormat()
{
    // test setting/getting the property
    QVERIFY(componentObject->setProperty("textFormat", Qt::AutoText));
    QCOMPARE(componentObject->property("textFormat").toInt(), (int)Qt::AutoText);

    componentObject->setProperty("textFormat", 4);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("textFormat").toInt()
            <= (int)Qt::LogText);

    // try to set a lower value
    componentObject->setProperty("textFormat", -1);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("textFormat").toInt()
            >= (int)Qt::PlainText);

    // try to set random value
    componentObject->setProperty("textFormat", qrand());
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("textFormat").toInt()
            <= (int)Qt::PlainText
            && componentObject->property("textFormat").toInt()
            >= (int)Qt::LogText);
}

void tst_quickcomponentstextarea::wrapMode()
{
    QString text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                 "Pellentesque accumsan lorem ante, a accumsan risus. Ut vulputate "
                 "cursus ligula nec pretium. Suspendisse cursus scelerisque augue "
                 "ut egestas. Vestibulum ante ipsum primis in faucibus orci luctus "
                 "et ultrices posuere cubilia Curae");

    // follow the same test procedure of QDeclarativeTextEdit: only test if the width doesn't change
    // when we set the wrap mode

    componentObject->setProperty("width", 300);
    componentObject->setProperty("text", "");

    QVERIFY(componentObject->setProperty("wrapMode", QTextOption::NoWrap));
    componentObject->setProperty("text", text);
    QCOMPARE(componentObject->property("width").toInt(), 300);
    componentObject->setProperty("text", "");

    QVERIFY(componentObject->setProperty("wrapMode", QTextOption::WordWrap));
    componentObject->setProperty("text", text);
    QCOMPARE(componentObject->property("width").toInt(), 300);
    componentObject->setProperty("text", "");

    QVERIFY(componentObject->setProperty("wrapMode", QTextOption::ManualWrap));
    componentObject->setProperty("text", text);
    QCOMPARE(componentObject->property("width").toInt(), 300);
    componentObject->setProperty("text", "");

    QVERIFY(componentObject->setProperty("wrapMode", QTextOption::WrapAnywhere));
    componentObject->setProperty("text", text);
    QCOMPARE(componentObject->property("width").toInt(), 300);
    componentObject->setProperty("text", "");

    // check if the enum stays in the right range
    componentObject->setProperty("wrapMode", 5);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("wrapMode").toInt()
            <= (int)Qt::LogText);

    // try to set a lower value
    componentObject->setProperty("wrapMode", -1);
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("wrapMode").toInt()
            >= (int)Qt::PlainText);

    // try to set random value
    componentObject->setProperty("wrapMode", qrand());
    QEXPECT_FAIL("", "Not yet blocked by enum range,"
                 "http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-414",
                 Continue);
    QVERIFY(componentObject->property("wrapMode").toInt()
            <= (int)Qt::PlainText
            && componentObject->property("wrapMode").toInt()
            >= (int)Qt::LogText);

}

void tst_quickcomponentstextarea::errorHighlight()
{
    QVERIFY(componentObject->setProperty("errorHighlight",false));
    QCOMPARE(componentObject->property("errorHighlight").toBool(),false);
    QVERIFY(componentObject->setProperty("errorHighlight",true));
    QCOMPARE(componentObject->property("errorHighlight").toBool(),true);
}

void tst_quickcomponentstextarea::copypaste()
{
    // set the text to something known, select and copy it
    componentObject->setProperty("text", "Hello Copy World");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "copy"), "Could not copy the text");

    // check if clipboard's content is right
    QCOMPARE(QApplication::clipboard()->text(), QString("Hello Copy World"));

    // clear text property
    componentObject->setProperty("text", "");
    QCOMPARE(componentObject->property("text").toString(), QString(""));

    // paste
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "paste"), "Could not paste the text");

    // check value of text
    QCOMPARE(componentObject->property("text").toString(), QString("Hello Copy World"));
}

void tst_quickcomponentstextarea::cutpaste()
{
    // set the text to something known, select and cut it
    componentObject->setProperty("text", "Hello Cut World");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "selectAll"), "Could not select all the text");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "cut"), "Could not cut the text");

    // check if clipboard's content is right
    QCOMPARE(QApplication::clipboard()->text(), QString("Hello Cut World"));

    // check if the text area was cleared
    QCOMPARE(componentObject->property("text").toString(), QString(""));

    // check if the cursor position is '0' (no text after all)
    QCOMPARE(componentObject->property("cursorPosition").toInt(), 0);

    // check if selection start and end are '0' too
    QCOMPARE(componentObject->property("selectionStart").toInt(), 0);
    QCOMPARE(componentObject->property("selectionEnd").toInt(), 0);

    // paste
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "paste"), "Could not paste the text");

    // check value of text
    QCOMPARE(componentObject->property("text").toString(), QString("Hello Cut World"));
}

void tst_quickcomponentstextarea::positionAt()
{
    // set the text to something known
    QVariant retVal;
    const QString text("Hello from Position World");
    componentObject->setProperty("text", text);
    componentObject->setProperty("cursorPosition", 0);
    const int vCenter = componentObject->property("height").toInt() / 2;

    // values smaller than the position of the first char will always return 0
    const QPointF smaller = textEdit->mapToItem(root, 0, 0);
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, smaller.x()),
                                       Q_ARG(QVariant, smaller.y())), "Could not call positionAt");
    QCOMPARE(retVal.toInt(), 0);

    // this test considers that the width of the element is really big
    // big enough to fit the text and still have some more space
    // values bigger than the position of the last char will always return the size of the string
    const int size = componentObject->property("width").toInt();

    const QPointF bigger = textEdit->mapToItem(root, size, vCenter);
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, bigger.x()),
                                       Q_ARG(QVariant, bigger.y())), "Could not call positionAt");
    QCOMPARE(retVal.toInt(), text.size());

    // the position returned for the pixel that is in the end of the text
    // should be the position of the last char
    QFontMetrics fm(mfont);
    const int width = fm.width(text);
    const QPointF mappedWidth = textEdit->mapToItem(root, width, vCenter);

    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, mappedWidth.x()),
                                       Q_ARG(QVariant, mappedWidth.y())), "Could not call positionAt");
    QCOMPARE(text.left(retVal.toInt()), text);
}

void tst_quickcomponentstextarea::positionToRectangle()
{
    // set the text to something known
    QVariant retVal;
    QVariant retValPos;
    const QString text("Hello from Rectangle World");
    componentObject->setProperty("text", text);
    componentObject->setProperty("cursorPosition", 0);

    // the cursor position should not change after a call to positionToRectangle()
    const int oldPosition = componentObject->property("cursorPosition").toInt();

    // now we take the rectangle and check if at the given position we have the right
    // char. because of this, positionToRectangle() should be tested *after* positionAt()
    // because we rely that it's working properly to check the right position
    const int size = text.size() / 2;
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionToRectangle",
                                       Q_RETURN_ARG(QVariant, retVal), Q_ARG(QVariant, size)),
                                       "Could not call positionToRectangle");

    const QRectF rect = retVal.toRectF();
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "positionAt",
                                       Q_RETURN_ARG(QVariant, retValPos), Q_ARG(QVariant, rect.x()),
                                       Q_ARG(QVariant, rect.y())), "Could not call positionAt");

    // it should be the same as used in 'value' (in this case size)
    QCOMPARE(retValPos.toInt(), size);

    // the 'x' of the rectangle should be the same as the width of the text
    // until the position returned
    QFontMetrics fm(mfont);
    QCOMPARE((int)root->mapRectToItem(textEdit, retVal.toRectF()).x(), fm.width(text.left(retValPos.toInt())));

    // the position shouldn't have changed
    QCOMPARE(oldPosition, componentObject->property("cursorPosition").toInt());

}

void tst_quickcomponentstextarea::inputMethodHints()
{
    QVERIFY( componentObject->setProperty("inputMethodHints", Qt::ImhPreferNumbers) );
    QCOMPARE( componentObject->property("inputMethodHints"), QVariant(Qt::ImhPreferNumbers) );

    QVERIFY( componentObject->setProperty("inputMethodHints", QVariant(Qt::ImhNoAutoUppercase | Qt::ImhUppercaseOnly | Qt::ImhNoPredictiveText)) );
    QCOMPARE( componentObject->property("inputMethodHints"), QVariant(Qt::ImhNoAutoUppercase | Qt::ImhUppercaseOnly | Qt::ImhNoPredictiveText) );
}

void tst_quickcomponentstextarea::softwareInputPanel()
{
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "openSoftwareInputPanel"), "Could not openSoftwareInputPanel");
    QVERIFY2(QMetaObject::invokeMethod(componentObject, "closeSoftwareInputPanel"), "Could not closeSoftwareInputPanel");
}

QTEST_MAIN(tst_quickcomponentstextarea)

#include "tst_quickcomponentstextarea.moc"
