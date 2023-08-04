#include "btclient.h"

#include <QIODevice>
#include <QFile>
#include <QTextStream>

#define SERIAL_FILE_PATH  "/etc/root/test.txt"
static const QBluetoothAddress testAddr("00:1A:7D:DA:71:15");
static const QBluetoothUuid testUuid((QString)"94f39d29-7d6d-437d-973b-fba39e49d4ee");
static const QString testUuidstr = "94f39d29-7d6d-437d-973b-fba39e49d4ee";

btclient::btclient(QObject *parent) : QObject(parent)
{

}

btclient::~btclient()
{
    stopClient();
}

void btclient::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;


    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    connect(socket, &QBluetoothSocket::connected, this, &btclient::connected);
    socket->connectToService(testAddr, testUuid, QIODevice::ReadWrite);
    qDebug() << "ConnectToService done";
}

/*
 To test sending data over bluetooth,
 the connected slot dumps the contents of
 our log file to a bluetooth server.
 It would be better design to have a class on
 top of the client that reads the log file and
 passes it to a "write" method in the client class.
*/
void btclient::connected()
{
    QFile inputFile(SERIAL_FILE_PATH);
    QString line;
    QByteArray temp;
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while(!in.atEnd())
        {
            line = in.readLine();
            temp = line.toUtf8() + '\n';
            socket->write(temp);
            qDebug() << "Sent: " << line;
        }
        inputFile.close();
    }
    socket->write("Done :)");


    socket->disconnectFromService();
    stopClient();
}

void btclient::stopClient()
{
    delete socket;
    socket = nullptr;
}
