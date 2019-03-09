#ifndef QREMOTESERVICE_H
#define QREMOTESERVICE_H

#include <QObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QMetaMethod>
#include <QDebug>
#include "private/qremoteservice_p.h"

/*******************************************************************
 *   Services have:
 *   A name - Unique to the Service
 *   A Type - Service type (used for classification)
 *   A Description - Short (or maybe long) human information
 *   A Function List (vTable) - describes service functionality
 *   A State(multiple) - describes changeable states
 *   A Set of Properties - describes data (read-only and volatile)
 *   A List of Enumerations - enumerated values
 *
 * Services will work through Byte messages encoded internally.
 * Developers work through the high level object API
 *
 ********************************************************************/

class QREMOTEOBJECT_EXPORT QRemoteService : public QRemoteObject
{
    Q_OBJECT
    Q_REMOTEOBJECT
    Q_PROPERTY(QString name READ serviceName)
    Q_PROPERTY(QString domain READ serviceDomain)
    Q_PROPERTY(QString description READ serviceDescription)
    Q_PROPERTY(QJsonObject directory READ objectDirectory)
    Q_PROPERTY(QString type READ serviceType)
public:
    QRemoteService(QObject * parent = nullptr);
    Q_INVOKABLE void  bindRemoteObject(QRemoteObject* obj, QRemoteObject *parent = nullptr);
    Q_INVOKABLE void  removeRemoteObject(QRemoteObject* obj);
    QJsonObject jsonReflection() override;
    Q_INVOKABLE QJsonObject readObject(quint32 id) const ;
    Q_INVOKABLE QJsonObject objectDirectory() const;
    Q_INVOKABLE QString serviceName()const ;
    Q_INVOKABLE QString serviceDomain() const;
    Q_INVOKABLE QString serviceDescription() const;
    Q_INVOKABLE QString serviceType() const;
    Q_INVOKABLE void sendActionList(ServiceActions::ActionList& list);

signals:
    //Q_INVOKABLE void receivedObjectMessage(quint32 id, QByteArray message);
    Q_REMOTE void boundNewObject(QJsonObject obj);
    Q_REMOTE void badRequestFromRemote(QJsonObject object);
    Q_REMOTE void receivedActionList(ServiceActions::ActionList & actions);

public slots:
    void setDomain(QString domain);

protected:
    QSharedDataPointer<QRemoteServicePrivate>  mServiceData;
protected slots:
    void fromReflection(QJsonObject data) override;
    Q_INVOKABLE void sendMessageToObject(quint32 id, QByteArray message);
    Q_INVOKABLE void processActionList(QByteArray list);
    void parseOutgoing(QByteArray message);
};


#endif // QREMOTESERVICE_H
