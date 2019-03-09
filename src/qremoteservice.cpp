#include "private/qremoteobject_p.h"
#include "qremoteobject.h"
#include "qremoteservice.h"
#include "qobject_serial.h"

QRemoteService::QRemoteService(QObject *parent)  : QRemoteObject(parent)
{
    mServiceData = new QRemoteServicePrivate();
}


void QRemoteService::fromReflection(QJsonObject data)
{
    const QMetaObject* my_meta = metaObject();
    QJsonArray name_map = data.value("object_map").toArray();
    for(int index(0); index < mServiceData->objects.length(); ++index)
    {
        QString name = name_map.at(index).toString();
        QRemoteObject * remote_obj = mServiceData->objects.at(index);
        if(remote_obj->objectName().length() != 0)
        {
            if(data.contains(name))
            {
                QJsonObject child_data = data.value(name).toObject();
                remote_obj->fromReflection(child_data);
            }
        }
        else
        {
            remote_obj->setObjectName(name);
            QJsonObject child_data = data.value(name).toObject();
            remote_obj->fromReflection(child_data);
        }
    }
    for(int index(0);index < my_meta->propertyCount(); index++){
        QMetaProperty property = my_meta->property(index);
        QString property_name = property.name();
        QJsonObject p_obj = data.value(property_name).toObject();
        if(p_obj.contains(QREMOTEOBJECT::QRO_CVALUE)){
            QVariant value = p_obj.value(QREMOTEOBJECT::QRO_CVALUE).toVariant();
            setProperty(property_name.toLocal8Bit().data(),value);
        }
    }
}

QJsonObject QRemoteService::jsonReflection()
{
    QJsonObject service = QREMOTEOBJECT::qobjectToJsonReflection(this);
    QJsonArray  name_map;
    for(auto remote_obj: mServiceData->objects)
    {
        service.remove(remote_obj->objectName());
        name_map.append(remote_obj->objectName());
        service.insert(remote_obj->objectName(), QREMOTEOBJECT:: qobjectToJsonReflection(remote_obj));
    }
    service.insert("object_map",name_map);
    return service;
}



void  QRemoteService::bindRemoteObject(QRemoteObject* obj, QRemoteObject * parent)
{
    if(!mServiceData->objects.contains(obj))
    {
        QJsonObject obj_desc = obj->jsonReflection();
        if(parent){
            if(!mServiceData->objects.contains(parent)){
                bindRemoteObject(parent);
            }
            QJsonObject parent_obj = mServiceData->retrieveFromDirectory(parent->objectName());
            parent_obj.insert(obj->objectName(), obj_desc);
            mServiceData->removeFromDirectory(parent->objectName());
            mServiceData->addToDirectory(parent_obj);
            mServiceData->objects.append(obj);
        }
        else{
            mServiceData->objects.append(obj);
            mServiceData->addToDirectory(obj_desc);
        }
        connect(obj, &QRemoteObject::dispatchNotification, this, &QRemoteService::parseOutgoing);
        emit boundNewObject(obj_desc);
    }
}

void QRemoteService::parseOutgoing(QByteArray message)
{
    QRemoteObject * obj = qobject_cast<QRemoteObject*>(sender());
    int id =mServiceData->objects.indexOf(obj);
    void* params[3] = {nullptr,&id, &message};
    QMetaMethod method;
    QByteArray data(QREMOTEOBJECT::Q_EncodeMetaCall(*this,method, this->metaObject()->methodOffset() +5,QMetaObject::InvokeMetaMethod,params));
    this->dispatchNotification(data);
}

void QRemoteService::processActionList(QByteArray list)
{
    ServiceActions::ActionList actions;
    actions.fromBinary(list);
    if(actions.actionCount() > 0){
        emit receivedActionList(actions);
    }
}

void  QRemoteService::removeRemoteObject(QRemoteObject* obj)
{
    if(!mServiceData->objects.contains(obj))
    {
        mServiceData->objects.removeOne(obj);
    }
}
QJsonObject QRemoteService::readObject(quint32 id) const
{
    return mServiceData->directory.value(QString::number(id)).toObject();
}

QJsonObject QRemoteService::objectDirectory() const
{
    return mServiceData->directory;
}

QString QRemoteService::serviceDescription() const
{
    return mServiceData->description;
}

QString QRemoteService::serviceDomain() const
{
    return mServiceData->domain;
}

QString QRemoteService::serviceName()const
{
    return this->objectName();
}

QString QRemoteService::serviceType() const
{
    return mServiceData->type;
}

void QRemoteService::sendMessageToObject(quint32 id, QByteArray message)
{
    if( int(id) < mServiceData->objects.length()){
        QRemoteObject* obj =  mServiceData->objects.at(id);
        if(obj){
            obj->processNotification(message);
        }
    }
}

void QRemoteService::sendActionList(ServiceActions::ActionList &list)
{
    QByteArray list_data = list.toBinary();
    void* params[] = {nullptr, &list_data};
    QMetaMethod method;
    QByteArray data(QREMOTEOBJECT::Q_EncodeMetaCall(*this,method, this->metaObject()->methodOffset() +6,QMetaObject::InvokeMetaMethod,params));
    this->dispatchNotification(data);
}

void QRemoteService::setDomain(QString domain)
{
    if(domain.compare(mServiceData->domain) != 0){
        mServiceData->domain = domain;
    }
}
