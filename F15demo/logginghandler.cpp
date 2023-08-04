#include "logginghandler.h"

extern QObject * topLevel;
#define SERIAL_FILE_PATH  "/etc/root/test.txt"

loggingHandler::loggingHandler(QObject *parent): QObject(parent)
{
    connect(&serialConsole, &SerialController::messageAvailable, this, &loggingHandler::serialConsoleLineReady);
}

loggingHandler::~loggingHandler()
{
    serialConsole.~SerialController();
}

void loggingHandler::f15GuiUpdate(QString msg)
{
    msg.insert(0, QString("GUI: "));
    logSerialData(msg);
    serialConsole.send(msg);
}

void loggingHandler::serialConsoleLineReady(QString msg)
{
    QString logMsg = "Serial: Torque Changed to ";
    QStringList serialList;
    QStringList::iterator it;
    QVariant returnedValue;
    QString temp, temp1;
    int torqueValue;

    // This sends torque value to QML program to dispaly on screen
    if(msg.contains(':'))
    {
        serialList = msg.split(':');
        it = serialList.begin();
        temp1 = *it;
        it++;
        temp = *it;
        if(temp1.contains("torque"))
        {
            logSerialData(logMsg.append(temp));
            torqueValue = temp.toInt();
            QMetaObject::invokeMethod(topLevel, "updateTorqueDisplay", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, torqueValue));
        }
    }
}

void loggingHandler::logSerialData(QString serialStr)
{
    QFile serialFile((QString)SERIAL_FILE_PATH);

    if (!serialFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream out(stdout);
        out << "CANNOT OPEN LOG FILE: " << SERIAL_FILE_PATH;
        return;
    }

    serialFile.seek(serialFile.size());
    QTextStream fileStream(&serialFile);
    fileStream << serialStr;
    fileStream << "\n";
}
