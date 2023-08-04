#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "logginghandler.h"
#include "btserver.h"
#include "btclient.h"
#include "btserviceselector.h"

QObject * topLevel;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
      qDebug() << "Engine LOAD failed";
      return -1;
    }

    qDebug() << "Engine LOAD complete";
    topLevel = engine.rootObjects().value(0);
    QQuickWindow * theWindow = qobject_cast < QQuickWindow * > (topLevel);

    if (theWindow == nullptr) {
      qDebug() << "Can't instantiate window";
    }

    loggingHandler logHandler;
    btServiceSelector serviceSelector;
    //btserver serverTest;
    btclient clientTest;


    bool success = QObject::connect(theWindow, SIGNAL(submitTextField(QString)), &logHandler, SLOT(f15GuiUpdate(QString)));
    Q_ASSERT(success);
    //success = QObject::connect(theWindow, SIGNAL(startBtServer()), &serverTest, SLOT(startServer()));
    //success = QObject::connect(theWindow, SIGNAL(startBtServer()), &clientTest, SLOT(startDiscoveryAgent()));
    success = QObject::connect(theWindow, SIGNAL(startBtServer()), &serviceSelector, SLOT(startServiceDiscovery()));
    Q_ASSERT(success);
    success = QObject::connect(&serviceSelector, SIGNAL(attemptClientConn(QBluetoothServiceInfo)), &clientTest, SLOT(startClient(QBluetoothServiceInfo)));
    Q_ASSERT(success);

    return app.exec();
}
