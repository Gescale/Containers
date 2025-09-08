#include "memento.h"

Memento::Memento(const QList<QVariantMap>& state) : _state(state) {}

QList<QVariantMap> Memento::getState() const {
    return _state;
}

Caretaker::Caretaker() : _memento(nullptr) {}

Caretaker::~Caretaker() {
    delete _memento;
}

void Caretaker::saveState(Memento* memento) {
    delete _memento;
    _memento = memento;
}

Memento* Caretaker::restoreState() {
    return _memento;
}
