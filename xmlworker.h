#ifndef XMLWORKER_H
#define XMLWORKER_H

#include <QObject>
#include <QMap>

class Pallet;

class XmlWorker : public QObject {
    Q_OBJECT

public:
    explicit XmlWorker(QObject *parent = nullptr);
    ~XmlWorker();

public slots:
    void process(const QMap<int, Pallet*>& pallets);

signals:
    void xmlGenerated(const QString& xmlString);
    void error(const QString& errorMessage);
    void finished();

private:
    QString generateXmlString(const QMap<int, Pallet*>& pallets);
};

#endif // XMLWORKER_H
