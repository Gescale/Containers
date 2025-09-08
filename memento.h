#ifndef MEMENTO_H
#define MEMENTO_H

#include <QList>
#include <QVariantMap>

// The Memento contains the state of an object to be restored.
class Memento {
public:
    explicit Memento(const QList<QVariantMap>& state);
    QList<QVariantMap> getState() const;

private:
    const QList<QVariantMap> _state;
};

// The Caretaker is responsible for the memento's safekeeping.
class Caretaker {
public:
    Caretaker();
    ~Caretaker();

    void saveState(Memento* memento);
    Memento* restoreState();

private:
    Memento* _memento;
};

#endif // MEMENTO_H
