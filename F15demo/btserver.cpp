#include "btserver.h"

#include <QtBluetooth/qbluetoothserver.h>
#include <QtBluetooth/qbluetoothsocket.h>

extern QObject * topLevel;
#define SERIAL_FILE_PATH  "/etc/root/test.txt"
QByteArray readData;

//! [Service UUID]
static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
//! [Service UUID]

btserver::btserver(QObject *parent) : QObject(parent)
{

}

btserver::~btserver()
{
    stopServer();
}

void btserver::startServer(const QBluetoothAddress& localAdapter)
{
    if (rfcommServer)
        return;

    //! [Create the server]
    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, &QBluetoothServer::newConnection,
            this, QOverload<>::of(&btserver::clientConnected));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning() << "Cannot bind chat server to" << localAdapter.toString();
        return;
    }
    //! [Create the server]

    //serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    QBluetoothServiceInfo::Sequence profileSequence;
    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    classId << QVariant::fromValue(quint16(0x100));
    profileSequence.append(QVariant::fromValue(classId));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             profileSequence);

    classId.clear();
    classId << QVariant::fromValue(QBluetoothUuid(serviceUuid));
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);

    //! [Service name, description and provider]
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Chat Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Example bluetooth chat server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));
    //! [Service name, description and provider]

    //! [Service UUID set]
    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));
    //! [Service UUID set]

    //! [Service Discoverability]
    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);
    //! [Service Discoverability]

    //! [Protocol descriptor list]
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);
    //! [Protocol descriptor list]

    //! [Register service]
    serviceInfo.registerService(localAdapter);
    //! [Register service]

    QVariant returnedValue;
    QMetaObject::invokeMethod(topLevel, "printBtUpdate", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, (QString)"Server Started."));

}

//! [stopServer]
void btserver::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);

    // Close server
    delete rfcommServer;
    rfcommServer = nullptr;
}
//! [stopServer]

//! [sendMessage]
void btserver::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';

    for (QBluetoothSocket *socket : qAsConst(clientSockets))
        socket->write(text);
}
//! [sendMessage]

//! [clientConnected]
void btserver::clientConnected()
{
    QString debugMsg = "Client Connected: ";
    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, &QBluetoothSocket::readyRead, this, &btserver::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, QOverload<>::of(&btserver::clientDisconnected));
    clientSockets.append(socket);
    QVariant returnedValue;
    QMetaObject::invokeMethod(topLevel, "printBtUpdate", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, debugMsg.append(socket->peerName())));

}
//! [clientConnected]

//! [clientDisconnected]
void btserver::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnected(socket->peerName());

    clientSockets.removeOne(socket);

    socket->deleteLater();
}
//! [clientDisconnected]

//! [readSocket]
void btserver::readSocket()
{

    uint64_t sizeRead;
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    sizeRead = socket->bytesAvailable();
    readData = socket->read(sizeRead);

    qDebug() << "Client Data read: " << readData.data();


    QFile inputFile(SERIAL_FILE_PATH);
    QString line;
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while(!in.atEnd())
        {
            line = in.readLine();
            sendMessage(line);
            qDebug() << "Sent: " << line;
        }
        inputFile.close();
    }
    sendMessage((QString)"Done :)");

    QVariant returnedValue;
    QMetaObject::invokeMethod(topLevel, "printBtUpdate", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, (QString)"Logs Dumped. Stopping Server"));
    stopServer();

}
//! [readSocket]
