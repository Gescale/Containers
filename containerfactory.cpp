#include "containerfactory.h"
#include <QDate>

ContainerFactory::ContainerFactory() = default;

Container* ContainerFactory::createContainer(const QString &type, int weight, int height, int dim1, int dim2) {
    if (type.toLower() == "box") {
        QString code = generateCode("B");
        return new Box(code, weight, height, dim1, dim2);
    } else if (type.toLower() == "cylinder") {
        QString code = generateCode("C");
        return new Cylinder(code, weight, height, dim1);
    }
    return nullptr;
}

QString ContainerFactory::generateCode(const QString &typeChar) {
    QDate currentDate = QDate::currentDate();
    int year = currentDate.year();
    int month = currentDate.month();
    int serial = (typeChar == "B") ? _boxSerial++ : _cylinderSerial++;

    return QString("%1/%2/%3%4")
        .arg(year, 4, 10, QChar('0'))
        .arg(month, 2, 10, QChar('0'))
        .arg(typeChar)
        .arg(serial);
}
