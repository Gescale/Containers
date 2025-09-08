#include "box.h"

Box::Box(QString code, int weight, int height, int length, int breadth, QObject *parent)
    : Container(code, weight, height, parent), _length(length), _breadth(breadth) {}

int Box::getVolume() const {
    return _length * _breadth * _height;
}

QString Box::getType() const {
    return "Box";
}

int Box::getLength() const {
    return _length;
}

int Box::getBreadth() const {
    return _breadth;
}
