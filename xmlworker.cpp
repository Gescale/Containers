#include "xmlworker.h"
#include "pallet.h"
#include <QXmlStreamWriter>
#include <QTcpSocket>
#include <QThread>
#include <QMetaProperty>

XmlWorker::XmlWorker(QObject *parent) : QObject(parent) {}

XmlWorker::~XmlWorker() = default;

void XmlWorker::process(const QMap<int, Pallet*>& pallets) {
    QString xmlString = generateXmlString(pallets);
    if (xmlString.isEmpty()) {
        emit error("No pallets to process.");
        emit finished();
        return;
    }

    emit xmlGenerated(xmlString);

    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 6164);
    if (socket.waitForConnected(3000)) {
        socket.write(xmlString.toUtf8());
        socket.waitForBytesWritten(1000);
        socket.disconnectFromHost();
    } else {
        emit error("Error: Could not connect to server.");
    }
    emit finished();
}

QString XmlWorker::generateXmlString(const QMap<int, Pallet*>& pallets) {
    QString xmlString;
    QXmlStreamWriter writer(&xmlString);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("pallets");
    writer.writeAttribute("NumberOfPallets", QString::number(pallets.size()));

    for (Pallet* pallet : pallets) {
        writer.writeStartElement("pallet");
        writer.writeAttribute("number", QString::number(pallet->getNumber()));
        writer.writeAttribute("weight", QString::number(pallet->getTotalWeight()));
        writer.writeAttribute("volume", QString::number(pallet->getTotalVolume()));

        for (Container* container : pallet->getContainers()) {
            const QMetaObject* metaObject = container->metaObject();
            writer.writeStartElement(container->getType());

            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                QMetaProperty metaProperty = metaObject->property(i);
                const char* name = metaProperty.name();
                if (strcmp(name, "objectName") == 0) continue;

                QVariant value = metaProperty.read(container);
                writer.writeTextElement(name, value.toString());
            }

            writer.writeEndElement(); // Box or Cylinder
        }
        writer.writeEndElement(); // pallet
    }

    writer.writeEndElement(); // pallets
    writer.writeEndDocument();

    return xmlString;
}
