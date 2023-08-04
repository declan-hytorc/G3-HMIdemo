#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QSettings>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>

class SerialController : public QObject
{
    Q_OBJECT
    //Q_Property()

public:
    //explicit SerialController(const QString serialFileLocation, QObject *parent = nullptr);
    explicit SerialController(QObject *parent = nullptr);
    ~SerialController();

    void send(QString msg);
signals:
    void messageAvailable(QString msg);

public slots:

    void onSerialReadyRead(void);

private slots:

private:
    QSerialPort m_port;
    QSettings m_settings;

};

#endif // SERIALCONTROLLER_H
