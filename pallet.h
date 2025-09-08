#ifndef PALLET_H
#define PALLET_H

#include "container.h"
#include <QList>

class Pallet {
public:
    explicit Pallet(int number = 0);
    ~Pallet();

    void addContainer(Container* container);
    int getNumber() const;
    int getTotalWeight() const;
    int getTotalVolume() const;
    const QList<Container*>& getContainers() const;

private:
    int _number;
    QList<Container*> _containers;
};

#endif // PALLET_H
