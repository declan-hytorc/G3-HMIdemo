#ifndef BTCLIENT_H
#define BTCLIENT_H

#include <QObject>

#include <QtCore/qmetaobject.h>
#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothuuid.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>
#include <QString>

QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)


class btclient : public QObject
{
    Q_OBJECT
public:
    explicit btclient(QObject *parent = nullptr);
    ~btclient();


    void stopClient();

public slots:
    void startClient(const QBluetoothServiceInfo &remoteService);
    void connected();

private:
    QBluetoothSocket *socket = nullptr;
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;

signals:

};

#endif // BTCLIENT_H
