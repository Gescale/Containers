#include "cylinder.h"

Cylinder::Cylinder(QString code, int weight, int height, int diameter, QObject *parent)
    : Container(code, weight, height, parent), _diameter(diameter) {}

int Cylinder::getVolume() const {
    double radius = _diameter / 2.0;
    return static_cast<int>(M_PI * radius * radius * _height);
}

QString Cylinder::getType() const {
    return "Cylinder";
}

int Cylinder::getDiameter() const {
    return _diameter;
}
