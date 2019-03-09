#include "qremoteobject_p.h"
#include "../qobject_serial.h"
#include <QDataStream>
#include <QMetaType>
#include <QMetaProperty>
#include <private/qmetaobjectbuilder_p.h>
#include <QSharedPointer>
QVariant QREMOTEOBJECT::executeMappedInvoke(QObject* object, int method_index, const QVariantList &args)
{
    QMetaMethod method(object->metaObject()->method(method_index));
    int arg_count(-1);
    if(args.length() > method.parameterCount()){
        arg_count = method.parameterCount();
    }
    else{
        arg_count = args.length();
    }
    QVariant ret_val;
    if(method.returnType() != QMetaType::Void)
    {
        switch(arg_count)
        {
            case 0:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val));
                break;
            }
            case 1:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]));
                break;
            }
            case 2:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]));

                break;
            }
            case 3:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]));
                break;
            }
            case 4:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]));
                break;
            }
            case 5:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]));
                break;
            }
            case 6:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]),
                                                                Q_ARG(QVariant,args[5]));
                break;
            }
            case 7:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]),
                                                                Q_ARG(QVariant,args[5]),
                                                                Q_ARG(QVariant,args[6]));
                break;
            }
            case 8:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]),
                                                                Q_ARG(QVariant,args[5]),
                                                                Q_ARG(QVariant,args[6]),
                                                                Q_ARG(QVariant,args[7]));
                break;
            }
            case 9:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]),
                                                                Q_ARG(QVariant,args[5]),
                                                                Q_ARG(QVariant,args[6]),
                                                                Q_ARG(QVariant,args[7]),
                                                                Q_ARG(QVariant,args[8]));
                break;
            }
            case 10:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]),
                                                                Q_ARG(QVariant,args[5]),
                                                                Q_ARG(QVariant,args[6]),
                                                                Q_ARG(QVariant,args[7]),
                                                                Q_ARG(QVariant,args[8]),
                                                                Q_ARG(QVariant,args[9]));
                break;
            }
            default:
            {
                method.invoke(object,Q_RETURN_ARG(QVariant,ret_val),Q_ARG(QVariant,args[0]),
                                                                Q_ARG(QVariant,args[1]),
                                                                Q_ARG(QVariant,args[2]),
                                                                Q_ARG(QVariant,args[3]),
                                                                Q_ARG(QVariant,args[4]),
                                                                Q_ARG(QVariant,args[5]),
                                                                Q_ARG(QVariant,args[6]),
                                                                Q_ARG(QVariant,args[7]),
                                                                Q_ARG(QVariant,args[8]),
                                                                Q_ARG(QVariant,args[9]));
                break;
            }
        }
    }
    else
    {
        switch(arg_count)
        {
            case 0:
            {
                method.invoke(object);
                break;
            }
            case 1:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]));
                break;
            }
            case 2:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]), Q_ARG(QVariant,args[1]));

                break;
            }
            case 3:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]));
                break;
            }
            case 4:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                              Q_ARG(QVariant,args[3]));
                break;
            }
            case 5:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                              Q_ARG(QVariant,args[3]),
                                                                              Q_ARG(QVariant,args[4]));
                break;
            }
            case 6:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                              Q_ARG(QVariant,args[3]),
                                                                              Q_ARG(QVariant,args[4]),
                                                                              Q_ARG(QVariant,args[5]));
                break;
            }
            case 7:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                               Q_ARG(QVariant,args[3]),
                                                                               Q_ARG(QVariant,args[4]),
                                                                               Q_ARG(QVariant,args[5]),
                                                                               Q_ARG(QVariant,args[6]));
                break;
            }
            case 8:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                               Q_ARG(QVariant,args[3]),
                                                                               Q_ARG(QVariant,args[4]),
                                                                               Q_ARG(QVariant,args[5]),
                                                                               Q_ARG(QVariant,args[6]),
                                                                               Q_ARG(QVariant,args[7]));
                break;
            }
            case 9:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                            Q_ARG(QVariant,args[3]),
                                                                            Q_ARG(QVariant,args[4]),
                                                                            Q_ARG(QVariant,args[5]),
                                                                            Q_ARG(QVariant,args[6]),
                                                                            Q_ARG(QVariant,args[7]),
                                                                            Q_ARG(QVariant,args[8]));
                break;
            }
            case 10:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                                              Q_ARG(QVariant,args[3]),
                                                                              Q_ARG(QVariant,args[4]),
                                                                              Q_ARG(QVariant,args[5]),
                                                                              Q_ARG(QVariant,args[6]),
                                                                              Q_ARG(QVariant,args[7]),
                                                                              Q_ARG(QVariant,args[8]),
                                                                              Q_ARG(QVariant,args[9]));
                break;
            }
            default:
            {
                method.invoke(object,Q_ARG(QVariant,args[0]),Q_ARG(QVariant,args[1]),Q_ARG(QVariant,args[2]),
                                                             Q_ARG(QVariant,args[3]),Q_ARG(QVariant,args[4]), Q_ARG(QVariant,args[5]),
                                                             Q_ARG(QVariant,args[6]),Q_ARG(QVariant,args[7]),Q_ARG(QVariant,args[8]),
                                                             Q_ARG(QVariant,args[9]));
                break;
            }
        }
    }
    return ret_val;
}

// TODO: implement returns, yeah its not quite signal/slots like but it is possible
void QREMOTEOBJECT::notifyRemoteReturnValue(const QObject& object, int id, void** return_value)
{
    Q_UNUSED(object) Q_UNUSED(id) Q_UNUSED(return_value)
   /* const QMetaObject* meta = object->metaObject();
    // meta->superClass will be QRemoteObject
    QMetaMethod notify_method(meta->superClass()->method(meta->superClass()->methodOffset())); // notify method takes a string func_name and QVariant return_value
    // fetch method name -
    QMetaMethod invoked_remote_method(meta->method(id));
    QString func_name(invoked_remote_method.name());
    notify_method.invoke(object,Qt::DirectConnection,Q_ARG(QString, func_name),Q_ARG(QVariant, return_value));*/
}

QByteArray QREMOTEOBJECT::Q_EncodeRemoteTransaction(int id, QMetaObject::Call c, QVariantList params)
{
    QByteArray ret_val;
    QJsonObject obj;
    QJsonArray    param_array = QJsonArray::fromVariantList(params);
    obj["i"] = id;
    obj["c"] = (int)c;
    obj["p"] = param_array;
    QJsonDocument doc;
    doc.setObject(obj);
    ret_val = doc.toBinaryData();
    return ret_val;
}

QByteArray QREMOTEOBJECT::Q_EncodeMetaCall(const QObject& object,QMetaMethod& method, int id, QMetaObject::Call c, void ** params)
{
    method =   object.metaObject()->method(id);
    QByteArray out;
    QDataStream ss(&out,QIODevice::WriteOnly);
    ss.setVersion(QDataStream::Qt_5_10);
    int c_in(c);
    QMetaType::save(ss,QMetaType::Int,&c_in); // call type
    QMetaType::save(ss,QMetaType::Int,&id);        // method id
    // next encode the parameters;
    if(params != nullptr){
        for(int index(0); index < method.parameterCount(); index++ ){
            #ifdef Q_NO_DEBUG
                    int type(method.parameterType(index));
                    if(!QMetaType::save(ss, type ,args[index+1])){
                        qDebug() << "Failed to save parameter of type: " << QMetaType::typeName(type);
                        qDebug() << "For QRemoteObject::" << method.methodSignature();
                    }
            #else
                    if(QMetaType::QByteArray == method.parameterType(index))
                    {
                        QByteArray* array = static_cast<QByteArray*>(params[index+1]);
                        int length =array->length();
                        QMetaType::save(ss,QMetaType::Int,&length);
                        ss.writeRawData(array->data(),array->length());
                    }
                    else if(QMetaType::QJsonObject == method.parameterType(index))
                    {
                        QJsonObject* obj = static_cast<QJsonObject*>(params[index+1]);
                        QJsonDocument doc;
                        doc.setObject(*obj);
                        QByteArray array = doc.toBinaryData();
                        int length =array.length();
                        QMetaType::save(ss,QMetaType::Int,&length);
                        ss.writeRawData(array.data(),length);
                    }
                    else{
                        QMetaType::save(ss,method.parameterType(index),params[index+1]);
                    }
            #endif
        }
    }
    return out;
}

QByteArray QREMOTEOBJECT::Q_EncodeMetaProperty(const QObject &object, int id, QMetaObject::Call c, void **params)
{
    QMetaProperty p  =   object.metaObject()->property(id);
    QByteArray out;
    QDataStream ss(&out,QIODevice::WriteOnly);
    ss.setVersion(QDataStream::Qt_5_10);
    int c_in(c);
    QMetaType::save(ss,QMetaType::Int,&c_in); // call type
    QMetaType::save(ss,QMetaType::Int,&id);        // method id
    if(c == QMetaObject::WriteProperty)
    {
        if(int(QMetaType::QByteArray) == int(p.type()))
        {
            QByteArray* array = static_cast<QByteArray*>(params[1]);
            int length =array->length();
            QMetaType::save(ss,QMetaType::Int,&length);
            ss.writeRawData(array->data(),array->length());
        }
        else{
            QMetaType::save(ss,p.type(),params[1]);
        }
    }
    return out;
}

QMetaMethod QREMOTEOBJECT::Q_DecodeMethodCall(const QObject& object, int& id, void**& params, QByteArray data )
{
    QMetaMethod method(object.metaObject()->method(id));
    if(method.isValid() && (QStringLiteral("Q_LOCAL").compare( method.tag()) == 0)){
        if(!QREMOTEOBJECT::generateVoidStarParams(data,method,params))
        {
            id = -1;
            params = nullptr;
        }
    }else
    {
        id = -1;
        params = nullptr;
    }
    return method;
}
void QREMOTEOBJECT::Q_DecodeRemoteTransaction(QByteArray data, int& id, QMetaObject::Call& c, QVariantList& params)
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(data);
    QJsonObject   obj(doc.object());
    id = obj["i"].toInt();
    c =  QMetaObject::Call(obj["c"].toInt());
    QJsonArray param_array = obj["p"].toArray();
    params = param_array.toVariantList();
}

void QREMOTEOBJECT::Q_ReleaseEncodedParamaters(QMetaMethod method, void**& params, QObject * obj)
{
    // next encode the parameters;
    for(int index(0); index < method.parameterCount(); index++ )
    {
            QMetaType::destroy(method.parameterType(index),params[index]);
    }
    params = nullptr;
}

QVariantList QREMOTEOBJECT::getObjectProperties(QObject* object)
{
    QVariantList  properties;
    const QMetaObject* meta = object->metaObject();
    int property_count = meta->propertyCount();
    for(int index(meta->propertyOffset()); index < property_count;  index++)
    {
        QMetaProperty property = meta->property(index);
        QString name(property.name());
        properties.append(name);
        QVariant  value = property.read(object);
        properties.append(value);
    }
    return properties;
}

QByteArray QREMOTEOBJECT::getPortableDescription(QObject* object)
{
    QByteArray data;
    auto meta = object->metaObject();
    QMetaObjectBuilder b(meta);
    data = b.toRelocatableData();
    return data;
}

QMetaObject QREMOTEOBJECT::getQObjectInstanceFromDescription(QByteArray data,QObject* parent)
{
    QMetaObject meta;
    if(parent)
    {
        auto pmeta = parent->metaObject();
        QMetaObjectBuilder::fromRelocatableData(&meta,pmeta, data);
    } else
    {
        const QMetaObject* pmeta(nullptr);
        QMetaObjectBuilder::fromRelocatableData(&meta,pmeta, data);
    }

    return meta;
}

void QREMOTEOBJECT::setObjectProperties(QObject* object,QVariantList params)
{
    // pares params and set properties
    QVariant var = params.at(0);
    QVariantList properties = var.toList();
    const QMetaObject* meta = object->metaObject();
    for(int index(0); index+1 < properties.length();)
    {
        int p_index = meta->indexOfProperty(properties.value(index).toString().toLocal8Bit().data());
        if(p_index > 0)
        {
            meta->property(p_index).write(object,properties.value(index+1));
            index += 2;
        }
    }
}


