#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QMap>
#include <QQmlContext>
#include <QFile>


constexpr quint16  KEY_SERVER_PORT = 4421;
constexpr char KEY_SERVER_ADDR[] = "127.0.0.1";
#include "../keyfob.h"
typedef QMap<QWebSocket*,QString> KeyfobMap;
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    // generate method for communication (a websocket in this example)
    QWebSocketServer* car_server = new QWebSocketServer(QStringLiteral("car_server"),QWebSocketServer::NonSecureMode, &app);
    KeyfobMap  cf;
    KeyfobMap* connected_fobs = &cf;
    QStringList valid_fobs;
    // add some valid fob IDs
    auto master_fob_interface = new Keyfob(&app); // tied to lifetime of connection
    valid_fobs << QStringLiteral("A3X1") << QStringLiteral("B2C3") << QStringLiteral("CZ19") << QStringLiteral("T100");
    if(car_server){
        if(car_server->listen(QHostAddress(KEY_SERVER_ADDR),KEY_SERVER_PORT)){
            qDebug() << QStringLiteral("Car Server listening on: ") << KEY_SERVER_ADDR << QStringLiteral(" with port: ") << KEY_SERVER_PORT;
            QObject::connect(car_server, &QWebSocketServer::newConnection, [=]{
                auto pending_connection = car_server->nextPendingConnection();
                //qDebug() << "Incomming connection from: " << pending_connection->peerAddress();
                connected_fobs->insert(pending_connection,QString());
                QObject::connect(pending_connection, &QWebSocket::binaryMessageReceived, master_fob_interface, &Keyfob::processNotification);
                QObject::connect(master_fob_interface, &Keyfob::dispatchNotification, pending_connection, &QWebSocket::sendBinaryMessage);
            });
        }
    }else{
        delete master_fob_interface;
        delete car_server;
        qDebug() << QStringLiteral("Could not create the websocket server, exiting car_server example");
        QCoreApplication::exit(-1);
    }

    engine.rootContext()->setContextProperty("ValidKeyfobs", valid_fobs);
    engine.rootContext()->setContextProperty("KeyfobInput",master_fob_interface);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    int ret_val =  app.exec();
    delete master_fob_interface;
    delete car_server;
    return ret_val;
}
