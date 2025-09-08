#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QString>

class Container : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString code READ getCode)
    Q_PROPERTY(int weight READ getWeight)
    Q_PROPERTY(int height READ getHeight)
    Q_PROPERTY(int volume READ getVolume)

public:
    explicit Container(QString code, int weight, int height, QObject *parent = nullptr);
    virtual ~Container() = default;

    QString getCode() const;
    int getWeight() const;
    int getHeight() const;
    virtual int getVolume() const = 0;
    virtual QString getType() const = 0;

protected:
    QString _code;
    int _weight;
    int _height;
};

#endif // CONTAINER_H
