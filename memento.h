#ifndef MEMENTO_H
#define MEMENTO_H

#include <QList>
#include <QVariantMap>

class Memento {
public:
    explicit Memento(const QList<QVariantMap>& state);
    QList<QVariantMap> getState() const;

private:
    const QList<QVariantMap> _state;
};

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
