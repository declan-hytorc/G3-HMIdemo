#ifndef LOGGINGHANDLER_H
#define LOGGINGHANDLER_H


#include "serialcontroller.h"

class loggingHandler : public QObject
{
    Q_OBJECT
private:
    SerialController serialConsole;
    void logSerialData(QString serialStr);

public slots:
    void serialConsoleLineReady(QString msg);
    void f15GuiUpdate(QString msg);

public:
    loggingHandler(QObject *parent = nullptr);
    ~loggingHandler();
};

#endif // LOGGINGHANDLER_H
