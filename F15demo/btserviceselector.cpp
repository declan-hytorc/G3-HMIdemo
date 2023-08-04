#include "btserviceselector.h"

//94f39d29-7d6d-437d-973b-fba39e49d4ee
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>

QT_USE_NAMESPACE

static const QString testUuidstr = "94f39d29-7d6d-437d-973b-fba39e49d4ee";
static const QBluetoothUuid testUuid(testUuidstr);

/*
Service Discovery works, but for some reason it fails to detect the UUID from my
server in pybluez. This is odd because service discovery detects other UUIDs from
the computer. For now, I have set the desired UUID in the client model to connect to.
Ideally we would just pass a service info object to the client to connect.

TODO: Figure out how to get Service Discovery to identify UUID.
*/

// TODO: QT has a way of finding this so we shouldn't need to hard set it.
const static QBluetoothAddress localAdapter("00:16:A4:55:22:AE");

btServiceSelector::btServiceSelector(QObject *parent) : QObject(parent)
{
    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(localAdapter);

    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));

}

void btServiceSelector::startServiceDiscovery(const QBluetoothUuid &uuidFilter)
{
    if(m_discoveryAgent->isActive())
    {
        qDebug() << "Already Discovering Devices. Exit";
        return;
    }
    //m_discoveryAgent->setUuidFilter(testUuid);
    if(uuidFilter != nullUuid)
    {
        m_discoveryAgent->setUuidFilter(uuidFilter);
        qDebug() << "UUID Filter Set";
    }
    // for some reason when this is set to "Full Discovery" it cannot find anything
    m_discoveryAgent->start();
    qDebug() << "Starting Discovery";
    attemptedClientConn = false;
}

void btServiceSelector::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Discovered Device: ";
    qDebug() << " Name: " << serviceInfo.device().name();
    qDebug() << " Addr: " << serviceInfo.device().address().toString();
    QList<QBluetoothUuid> UUID_list;
    QList<QBluetoothUuid>::iterator it;

    QString temp;
    QBluetoothUuid tempU;

    UUID_list = serviceInfo.device().serviceUuids();
    for(it = UUID_list.begin(); it != UUID_list.end(); it++)
    {
        tempU = *it;
        temp = tempU.toString();

        qDebug() << "\tUUID: " << temp;

    }

    // If the name we are interested in is found
    // set boolean and save the corresponding service,
    // but wait until service discovery is over to connect to it.
    if( serviceInfo.device().name() == "reach-nxp-qt5-dev-vm")
    {
        if(!attemptedClientConn)
        {
            qDebug() << "Desired Service Found";
            attemptedClientConn = true;
            m_service = serviceInfo;
        }
    }
}

void btServiceSelector::discoveryFinished()
{
    qDebug() << "Discovery Finished";
    if(attemptedClientConn) emit attemptClientConn(m_service);
    m_discoveryAgent->clear();
}




