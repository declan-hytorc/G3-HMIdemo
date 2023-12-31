#ifndef BTSERVER_H
#define BTSERVER_H

#include <QObject>

#include <QtBluetooth/qbluetoothaddress.h>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QFile>
#include <QTextStream>
#include <QString>

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class btserver : public QObject
{
    Q_OBJECT
public:
    explicit btserver(QObject *parent = nullptr);
    ~btserver();

    void stopServer();

public slots:
    void sendMessage(const QString &message);
    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());

signals:
    void messageReceived(const QString &sender, const QString &message);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);

private slots:
    void clientConnected();
    void clientDisconnected();
    void readSocket();

private:
    QBluetoothServer *rfcommServer = nullptr;
    QBluetoothServiceInfo serviceInfo;
    QList<QBluetoothSocket *> clientSockets;

};

#endif // BTSERVER_H
