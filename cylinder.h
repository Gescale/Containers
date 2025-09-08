#ifndef CYLINDER_H
#define CYLINDER_H

#include "container.h"
#include <QtMath>

class Cylinder : public Container {
    Q_OBJECT
    Q_PROPERTY(int diameter READ getDiameter)

public:
    explicit Cylinder(QString code, int weight, int height, int diameter, QObject *parent = nullptr);
    int getVolume() const override;
    QString getType() const override;
    int getDiameter() const;

private:
    int _diameter;
};

#endif // CYLINDER_H
