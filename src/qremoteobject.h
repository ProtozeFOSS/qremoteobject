#ifndef QREMOTEOBJECT_H
#define QREMOTEOBJECT_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QObject>
#include <QMetaEnum>
#include <QMetaMethod>
#include <QMetaObject>
#include <QJsonParseError>
#include <QByteArray>
#include <QJsonArray>
#include "private/qremoteobject_p.h"

class QRemoteObject : public QObject
{
    Q_OBJECT
    Q_ENUMS(RpcCall)
public:
    friend class QRemoteService;
    QRemoteObject(QObject* parent = nullptr);
    virtual QJsonObject   jsonReflection();
    virtual QJsonObject   remoteReflection();
    ~QRemoteObject(){}
signals:
    void receivedRemoteReturnValue(QString func_name, QVariant return_value);
    void dispatchNotification(QByteArray notification);

public slots:
    void processNotification(const QByteArray &notification);
    void remoteInvokeMethod(QByteArray signature, QVariantList params);
    void initializeRemote();

 protected:
    virtual void fromReflection(QJsonObject data);
    static void  qt_remote_metacall(QObject* obj, QMetaObject::Call _c, int _id, void ** _a);

};



#endif // QREMOTEOBJECT_H
