#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QWebSocket>
#include <QQmlContext>
#include "../keyfob.h"
int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QWebSocket connection;
    Keyfob   fob;

    connection.open(QUrl("ws://127.0.0.1:4421"));
    QObject::connect(&connection, &QWebSocket::binaryMessageReceived, &fob, &Keyfob::processNotification);
    QObject::connect(&fob, &Keyfob::dispatchNotification, &connection, &QWebSocket::sendBinaryMessage);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("CarConnection",&connection);
    engine.rootContext()->setContextProperty("KeyfobObject",&fob);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
