#include "pallet.h"

Pallet::Pallet(int number) : _number(number) {}

Pallet::~Pallet() {
    qDeleteAll(_containers);
}

void Pallet::addContainer(Container* container) {
    if (container) {
        _containers.append(container);
    }
}

int Pallet::getNumber() const {
    return _number;
}

int Pallet::getTotalWeight() const {
    int total = 0;
    for (const auto& container : _containers) {
        total += container->getWeight();
    }
    return total;
}

int Pallet::getTotalVolume() const {
    int total = 0;
    for (const auto& container : _containers) {
        total += container->getVolume();
    }
    return total;
}

const QList<Container*>& Pallet::getContainers() const {
    return _containers;
}
