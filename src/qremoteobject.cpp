#include "qremoteobject.h"
#include "qobject_serial.h"
#include <QVariant>
QRemoteObject::QRemoteObject(QObject *parent)
    :QObject(parent)
{
    setParent(parent);
}

void QRemoteObject::fromReflection(QJsonObject data)
{
    const QMetaObject* my_meta = metaObject();
    QJsonArray jsn_properties = data.value(QREMOTEOBJECT::QRO_PROPS).toArray();
    for( auto property_in : jsn_properties){
        QJsonObject prop_obj_data = property_in.toObject();
        QString name_in = prop_obj_data.value(QREMOTEOBJECT::QRO_NAME).toString();
       QByteArray binary_name = name_in.toLocal8Bit();
        int index = my_meta->indexOfProperty(binary_name.data());
        QMetaProperty property = my_meta->property(index);
        if(property.isValid()){
            if(prop_obj_data.contains(QREMOTEOBJECT::QRO_CVALUE)){
                QVariant value = prop_obj_data.value(QREMOTEOBJECT::QRO_CVALUE).toVariant();
                setProperty(binary_name.data(),value);
            }
        }
    }
}

void QRemoteObject::initializeRemote()
{
    QByteArray out;
    QDataStream ss(&out,QIODevice::WriteOnly);
    ss.setVersion(QDataStream::Qt_5_10);
    int c_in(QREMOTEOBJECT::QueryRemoteObject);
    int id(-1);
    QMetaType::save(ss,QMetaType::Int,&c_in); // call type
    QMetaType::save(ss,QMetaType::Int,&id);
     emit dispatchNotification(out);
    //qDebug() << "Sent Initialize Request";
}

QJsonObject QRemoteObject::jsonReflection()
{
    return QREMOTEOBJECT::qobjectToJsonReflection(this);
}

QJsonObject QRemoteObject::remoteReflection()
{
    return QREMOTEOBJECT::qobjectToRemoteReflection(this);
}



void  QRemoteObject::qt_remote_metacall(QObject* obj, QMetaObject::Call _c, int _id, void ** _a)
{
    QRemoteObject* robj = qobject_cast<QRemoteObject*>(obj);
     if(robj != nullptr){
         QByteArray data;
         if(_c == QMetaObject::InvokeMetaMethod || _c == QMetaObject::RemoteInvokeMetaMethod ){
             QMetaMethod m;
             _id += obj->metaObject()->methodOffset();
              data = QREMOTEOBJECT::Q_EncodeMetaCall(*obj,m,_id,_c,_a);
         }
         else if(_c == QMetaObject::WriteProperty || _c == QMetaObject::ReadProperty)
         {
             _id += obj->metaObject()->propertyOffset();
             data = QREMOTEOBJECT::Q_EncodeMetaProperty(*obj,_id,_c,_a);
         }
        robj->dispatchNotification(data);
    }
}

void QRemoteObject::processNotification(QByteArray notification)
{
    void ** params;
    int c_val(-1);
    int id(-1);
    QDataStream ds(&notification,QIODevice::ReadOnly);
    ds.setVersion(QDataStream::Qt_5_10);
    ds >> c_val;
    ds >> id;
    switch(c_val)
    {
        case QMetaObject::InvokeMetaMethod:
        case QMetaObject::RemoteInvokeMetaMethod:
        {
            notification.remove(0,sizeof(int)*2);
            QMetaMethod method(QREMOTEOBJECT::Q_DecodeMethodCall((*this),id,params,notification));
            if(id > 0 ){
                id -= this->metaObject()->methodOffset();
                qt_metacall(QMetaObject::RemoteInvokeMetaMethod, id,params);
                QREMOTEOBJECT::releaseVoidStarParams(method,params);
            }
            break;
        }
        case QREMOTEOBJECT::RpcCall::ReturnInvokeMethod:
        {
            QMetaMethod method(QREMOTEOBJECT::Q_DecodeMethodCall((*this),id,params,notification));
            QREMOTEOBJECT::notifyRemoteReturnValue(*this,id,params);
            break;
        }
        case QREMOTEOBJECT::RpcCall::QueryRemoteObject:
        {
            QJsonObject reflection = this->jsonReflection();
            QByteArray out;
            int c = int(QREMOTEOBJECT::RpcCall::Construct);
            int id(-1);
            QDataStream ss(&out,QIODevice::WriteOnly);
            ss.setVersion(QDataStream::Qt_5_10);
            QMetaType::save(ss,QMetaType::Int,&c);
            QMetaType::save(ss,QMetaType::Int,&id);
            QJsonDocument doc;
            doc.setObject(reflection);
            QByteArray reflection_json = doc.toBinaryData();
            ss.writeRawData(reflection_json.data(),reflection_json.length());
            //qDebug() << "Received Initialize request " << objectName();
            this->dispatchNotification(out);
            break;
        }
        case QREMOTEOBJECT::RpcCall::Construct:
        {
          // decode right here
          notification.remove(0,sizeof(int)*2);
          QJsonDocument doc = QJsonDocument::fromBinaryData(notification);
          QJsonObject reflection = doc.object();
          this->fromReflection(reflection);
          break;
        }
        case QMetaObject::WriteProperty:
        {
            QMetaProperty p = this->metaObject()->property(id);
            if(p.isValid() && p.isWritable()){
                if(p.hasStdCppSet()){
                    //method = object.metaObject()->method();
                    //QREMOTEOBJECT::generateVoidStarParams(data,type,params);
                }
                else{
                    void* write_val(nullptr);
                    write_val = QMetaType::create(p.type());
                    //QVariant value(method.parameterType(index), params[index]);
                    if(QMetaType::load(ds,p.type(),write_val))
                    {
                        QVariant d(p.type(),write_val);
                        p.write(this,d);
                    }
                    QMetaType::destroy(p.type(),write_val);
                }
            }
            else{
                id = -1;
            }
            break;
        }
        case QMetaObject::ReadProperty:
        {
            //id += this->metaObject()->propertyOffset();
            QMetaProperty p = this->metaObject()->property(id);
            if(p.isReadable() && p.hasNotifySignal()) // need a signal to send the value back as
            {
                QVariant value = p.read(this);
                void* params[] = {nullptr,value.data()};
                auto sig = p.notifySignal();
                id =this->metaObject()->indexOfMethod(sig.methodSignature());
                auto out = QREMOTEOBJECT::Q_EncodeMetaCall((*this),sig,id,QMetaObject::RemoteInvokeMetaMethod,params);
                //qDebug() << "Received Initialize request " << objectName();
                this->dispatchNotification(out);
                break;
            }
            else {
                id = -1;
            }
            break;
        }
        default: break;
    }
}
void QRemoteObject::remoteInvokeMethod(QByteArray signature, QVariantList params)
{
    const QMetaObject * meta = metaObject();
    int method_index = meta->indexOfMethod(signature);
    emit dispatchNotification(QREMOTEOBJECT::Q_EncodeRemoteTransaction(method_index,QMetaObject::InvokeMetaMethod,params));
}
