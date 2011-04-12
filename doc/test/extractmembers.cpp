#include <QApplication>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDeclarativeListProperty>
#include <QDebug>

QString parseProperty(QMetaProperty * property, QMap<QString,QString> typemap, QString componentname)
{
    static QString propertytext = QString::null;
    // get property name
    QString name(property->name());
    if (name.startsWith("__"))   // ignore private properties
        return propertytext;

    // get property type and tidy up the format
    QString type(property->typeName());
    type.remove(0,type.lastIndexOf("::"));  //remove the property type namespace. e.g. QVariant::Bool, remove "QVariant::"
    int extension = type.indexOf("_QML");   // Remove any _QML... part
    if (extension != -1)
        type = type.left(extension);

    //change Qt C++ names to their equivalent
    type.replace("QDeclarative","");
    type.replace("*",""); // don't need pointer information

    if ( !typemap.value(type).isEmpty() )
        type = typemap.value(type);
    else if (type.startsWith("ListProperty"))    //needed because the names come appended with ListProperty.
        type.replace("ListProperty", "list");

    propertytext = type + " " + componentname + "::" + name;
    return propertytext;
}

QString parseMethod(QMetaMethod * method, QMap<QString,QString> typemap, QString componentname)
{
    QString methodtext = QString::null;
    // get the method signature and get it ready for the next processing
    QString methodsignature = method->signature();
    if (methodsignature.startsWith("__")) // ignore private methods
        return methodtext;
    if (methodsignature.endsWith("Changed()")) // ignore auto-signals
        return methodtext;
    methodsignature.resize(methodsignature.indexOf("(")+1);

    if(method->methodType() == QMetaMethod::Signal)
        methodtext.append("signal ");

    // only get the parameter names (QML doesn't care about the types)
    QList<QByteArray> paramNames = method->parameterNames();

    int size = paramNames.size();
    for (int i = 0; i < size; ++i) {
        methodsignature.append(QString(paramNames.at(i)));
        if (size != 1 && i != size - 1)
            methodsignature.append(", ");
    }
    methodsignature.append(")");
    methodtext.append(componentname + "::" + methodsignature);
    return methodtext;
}

//void parseEnumerator(QMetaEnum * metaEnum)
//{
//    QString enumStr = "enumeration ";
//    enumStr.append(itemName + "::" + metaEnum->name());
//    enumList<<enumStr;
//}

int main(int argc, char *argv[])
{
    // Get options from the command line
    QApplication a(argc, argv);
    QTextStream qout(stdout);
    if (argc < 2) {
        qout << "Usage: " << argv[0] << " <qmlfile> [outputfolder]";
        return(1);
    }
    // source qml file
    QFileInfo qmlfile(argv[1]);
    if (!qmlfile.exists()) {
        qout << "File not found: " << qmlfile.filePath();
        return (1);
    }
    // output folder
    QFileInfo outputfolder("");
    if (argc == 3) {
        outputfolder.setFile(argv[2]);
        if (!outputfolder.isDir()) {
            qout << "Output folder doesn't exist: " << outputfolder.filePath();
            return (1);
        }
    }

    // Parse the qml file
    QDeclarativeEngine* qmlEngine = new QDeclarativeEngine();
    qmlEngine->addPluginPath("../../imports/com/nokia/symbian"); // component's own imports specified fully so point directly to import folder where dll built to
    //qmlEngine->addImportPath("../../imports");
    QDeclarativeComponent* qmlComponent = new QDeclarativeComponent(qmlEngine, QUrl::fromLocalFile(qmlfile.filePath()));
    if (qmlComponent->isError()) {
        qDebug() << qmlComponent->errors();
        return (1);
        }
    QObject* qmlObject = qmlComponent->create();
    if ( !qmlObject ) {
        qout << "Failed to load: " << qmlfile.path();
        qDebug() << qmlComponent->errors();
        return (1);
    }

    //explicit mapping of Qt types to QML types
    //it's too complicated to scan through the basic types using private headers so hard-coding this
    QMap<QString,QString> qmlBasicType;
    qmlBasicType.insert("QAction",                      "action");
    qmlBasicType.insert("bool",                         "bool");
    qmlBasicType.insert("QColor",                       "color");
    qmlBasicType.insert("QDateTime",                    "date");
    qmlBasicType.insert("double",                       "double");
    qmlBasicType.insert("enumeration",                  "enumeration");
    qmlBasicType.insert("QFont",                        "font");
    qmlBasicType.insert("int",                          "int");
    qmlBasicType.insert("ListProperty",                 "list");
    qmlBasicType.insert("QPointF",                      "point");
    qmlBasicType.insert("qreal",                        "real");
    qmlBasicType.insert("QRectF",                       "rect");
    qmlBasicType.insert("QSize",                        "size");
    qmlBasicType.insert("QString",                      "string");
    qmlBasicType.insert("QDateTime",                    "time"); //Duplicate...
    qmlBasicType.insert("QUrl",                         "url");
    qmlBasicType.insert("QVariant",                     "variant");
    qmlBasicType.insert("TransformOrigin",              "enumeration");
    qmlBasicType.insert("QVector3d",                    "vector3d");
    qmlBasicType.insert("QGraphicsObject",              "Item");
    qmlBasicType.insert("SStyleWrapper",                "style");
    qmlBasicType.insert("QObject",                      "QtObject");
    qmlBasicType.insert("QValidator",                   "validator");
    qmlBasicType.insert("SDeclarativeImplicitSizeItem", "Item");
    qmlBasicType.insert("ListProperty<QObject>",        "list<QtObject>");
    qmlBasicType.insert("ListProperty<QGraphicsObject>","list<Item>");


    // Get the component name and remove the _QMLTYPE part
    QString componentname(qmlObject->metaObject()->className());
    componentname =  componentname.left(componentname.indexOf("_QML"));

    QStringList memberlist;

    // Collect all the properties (excluding inherited)
    for(int i = qmlObject->metaObject()->propertyOffset() ; i < qmlObject->metaObject()->propertyCount(); ++i) {
        memberlist.append(parseProperty(&qmlObject->metaObject()->property(i),qmlBasicType,componentname));
    }

    // Collect all the methods (excluding inherited)
    for(int i = qmlObject->metaObject()->methodOffset() ; i < qmlObject->metaObject()->methodCount(); ++i) {
        memberlist.append(parseMethod(&qmlObject->metaObject()->method(i),qmlBasicType,componentname));
    }

    // Output the results
    memberlist.sort();
    if (outputfolder.exists()) { // output to file
        QFile outputfile(outputfolder.filePath() + "/" + componentname.toLower() + ".txt");
        if (outputfile.open(QFile::WriteOnly)) {
            QTextStream fout(&outputfile);
            for (int i=0; i<memberlist.size();i++) {
                if (memberlist.at(i) != QString::null)
                    fout << memberlist.at(i) << "\n";
            }
        }
        else {
            qout << "Failed to open: " << outputfile.fileName();
            return(1);
        }
        outputfile.close();
    }
    else { // output to console
        for (int i=0; i<memberlist.size();i++) {
            if (memberlist.at(i) != QString::null)
                qout << memberlist.at(i) << "\n";
        }
    }

}
