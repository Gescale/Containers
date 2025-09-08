#include "container.h"

Container::Container(QString code, int weight, int height, QObject *parent)
    : QObject(parent), _code(code), _weight(weight), _height(height) {}

QString Container::getCode() const {
    return _code;
}

int Container::getWeight() const {
    return _weight;
}

int Container::getHeight() const {
    return _height;
}
