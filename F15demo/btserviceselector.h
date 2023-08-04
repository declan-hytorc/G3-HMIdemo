#ifndef BTSERVICESELECTOR_H
#define BTSERVICESELECTOR_H

#include <QObject>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothuuid.h>


QT_FORWARD_DECLARE_CLASS(QBluetoothServiceDiscoveryAgent)

QT_USE_NAMESPACE


static const QString nullUuidStr = "00000000-0000-0000-0000-000000000000";
static const QBluetoothUuid nullUuid(nullUuidStr);

class btServiceSelector : public QObject
{
    Q_OBJECT
public:
    explicit btServiceSelector(QObject *parent = nullptr);

signals:
    void passOnDiscoveredService(const QBluetoothServiceInfo &serviceInfo);
    void attemptClientConn(const QBluetoothServiceInfo &remoteService);

public slots:
    void startServiceDiscovery(const QBluetoothUuid &uuidFilter = nullUuid);

//private slots:
    // When a service is discovered this function will be called
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    // void discoveryError(QBluetoothServiceDiscoveryAgent::Error error);

private:
    // This is the object searching for services.
    QBluetoothServiceDiscoveryAgent* m_discoveryAgent;
    // This will hold information about a service
    QBluetoothServiceInfo m_service;
    bool attemptedClientConn;

};

#endif // BTSERVICESELECTOR_H
