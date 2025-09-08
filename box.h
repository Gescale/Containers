#ifndef BOX_H
#define BOX_H

#include "container.h"

class Box : public Container {
    Q_OBJECT
    Q_PROPERTY(int length READ getLength)
    Q_PROPERTY(int breadth READ getBreadth)

public:
    explicit Box(QString code, int weight, int height, int length, int breadth, QObject *parent = nullptr);
    int getVolume() const override;
    QString getType() const override;
    int getLength() const;
    int getBreadth() const;

private:
    int _length;
    int _breadth;
};

#endif // BOX_H
