#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtTest/QtTest>

#include "tst_quickcomponentstest.h"

class ApiCheckBase : public QObject
{
public:
    ApiCheckBase(QObject *parent = 0)
        : QObject(parent) {}

protected:
    void init(const QString &file);
    void validateProperty(const QString &propertyName, QVariant::Type propertyType, const QVariant &value = QVariant()) const;
    void validateSignal(const char *signalName) const;
    void validateMethod(const char *methodName) const;

private:
    QObject *componentObject;
};

void ApiCheckBase::init(const QString &file)
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile(file, &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void ApiCheckBase::validateProperty(const QString &propertyName, QVariant::Type propertyType, const QVariant &value) const
{
    const QMetaObject *metaObject = componentObject->metaObject();
    const int propertyIndex = metaObject->indexOfProperty(propertyName.toStdString().c_str());

    const QString propertyErrorString = QString("API Error: %1 not found").arg(propertyName);
    QVERIFY2(propertyIndex != -1, propertyErrorString.toStdString().c_str());

    const QMetaProperty metaProperty = metaObject->property(propertyIndex);

    const QString type = QVariant::typeToName(propertyType);
    const QString typeErrorString = QString("Type error: %1 is not a %2").arg(propertyName).arg(type);
    QVERIFY2(metaProperty.type() == propertyType, typeErrorString.toStdString().c_str());

    if (value.isValid()) {
        const QVariant tst = metaProperty.read(componentObject);
        const QString defaultErrorString = QString("Wrong default value");
        QVERIFY2(tst == value, defaultErrorString.toStdString().c_str());
    }
}

void ApiCheckBase::validateSignal(const char *signalName) const
{
    const QMetaObject *metaObject = componentObject->metaObject();
    const int signalIndex = metaObject->indexOfSignal(signalName);
    QVERIFY2(signalIndex != -1, "API Error: signal not found");
}

void ApiCheckBase::validateMethod(const char *methodName) const
{
    const QMetaObject *metaObject = componentObject->metaObject();
    const int methodIndex = metaObject->indexOfMethod(methodName);
    QVERIFY2(methodIndex != -1, "API Error: method not found");
}

