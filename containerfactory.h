#ifndef CONTAINERFACTORY_H
#define CONTAINERFACTORY_H

#include "box.h"
#include "cylinder.h"
#include <QString>

class ContainerFactory {
public:
    ContainerFactory();
    Container* createContainer(const QString &type, int weight, int height, int dim1, int dim2 = 0);

private:
    QString generateCode(const QString &typeChar);
    int _boxSerial = 1;
    int _cylinderSerial = 1;
};

#endif // CONTAINERFACTORY_H
