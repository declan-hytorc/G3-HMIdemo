#include <QtDebug>
#include "serialcontroller.h"

extern QObject * topLevel;

#define APP_SETTINGS_PATH "/data/etc/demos/settings.conf"


QByteArray rxBytes;

SerialController::SerialController(QObject *parent): QObject(parent), m_settings(APP_SETTINGS_PATH, QSettings::NativeFormat)
{
    qDebug() << "[Serial] Constructor";

    m_port.setPortName((QString)"/dev/ttymxc1" );
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setFlowControl(QSerialPort::NoFlowControl);
    m_port.setBaudRate(m_port.Baud115200, m_port.AllDirections);

    if (m_port.open(QIODevice::ReadWrite))
    {
        connect(&m_port, &QSerialPort::readyRead, this, &SerialController::onSerialReadyRead);
        qDebug() << "Serial port: " << m_port.portName() << " is up";
    }
    else
    {
        qDebug() << "Serial port error: Could not open" << m_port.portName() << " : " << m_port.errorString();
    }

}

SerialController::~SerialController()
{
    m_port.close();
}

void SerialController::send(QString msg)
{
    m_port.write(msg.append("\n\r").toUtf8());
}

// This is basically a callback function that executes every time
// a byte is sent over a serial connection. When the "\r" character
// is sent, the aray of bytes will be read, logged, and flushed out.
void SerialController::onSerialReadyRead()
{
    rxBytes.append(m_port.readAll());
    if (!rxBytes.contains("\r"))
    {
        return;
    }
    rxBytes = rxBytes.trimmed();
    QString serialData = rxBytes.data();
    messageAvailable(serialData);

    QVariant returnedValue;
    QMetaObject::invokeMethod(topLevel, "getSerial", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, rxBytes));

    // Make sure to flush rxBytes array before leaving
    rxBytes.clear();
}

