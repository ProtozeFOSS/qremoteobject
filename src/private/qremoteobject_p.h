#ifndef QREMOTEOBJECT_P_H
#define QREMOTEOBJECT_P_H
#include <QJsonArray>
#include <QMetaMethod>
#include <QMetaObject>
#if defined(QREMOTEOBJECT_LIBRARY)
#  define QREMOTEOBJECT_EXPORT Q_DECL_EXPORT
#else
#  define QREMOTEOBJECT_EXPORT Q_DECL_IMPORT
#endif

 // define the tag text as empty, so the compiler doesn't see it

#ifndef Q_MOC_RUN
    #define Q_REMOTEOBJECT
#endif

#ifndef Q_MOC_RUN
    #define Q_REMOTE
#endif

#ifndef Q_MOC_RUN
    #define Q_REMOTEONLY
#endif

#ifndef Q_MOC_RUN
    #define Q_LOCAL
#endif

namespace QREMOTEOBJECT
{
     QREMOTEOBJECT_EXPORT QVariant executeMappedInvoke(QObject* object, int method_index, const QVariantList &args);

     QREMOTEOBJECT_EXPORT void notifyRemoteReturnValue(const QObject& object, int id, void** return_value);

     QREMOTEOBJECT_EXPORT QByteArray Q_EncodeRemoteTransaction(int id, QMetaObject::Call c, QVariantList params);

     QREMOTEOBJECT_EXPORT QByteArray Q_EncodeMetaCall(const QObject& object,QMetaMethod& method,int id, QMetaObject::Call c, void ** params);

     QREMOTEOBJECT_EXPORT QByteArray Q_EncodeMetaProperty(const QObject & object, int id,  QMetaObject::Call c,  void ** params);

     QREMOTEOBJECT_EXPORT QMetaMethod Q_DecodeMethodCall(const QObject& object,  int& id, void**& params, QByteArray data );

     QREMOTEOBJECT_EXPORT void Q_DecodeRemoteTransaction(QByteArray data, int& id, QMetaObject::Call& c, QVariantList& params);

     QREMOTEOBJECT_EXPORT void Q_ReleaseEncodedParamaters(QMetaMethod method, void**& params, QObject * obj);
    #define Q_EMIT_REMOTE_SIGNAL(id, params) \
    dispatchNotification(Q_EncodeRemoteTransaction(id,QMetaObject::InvokeMetaMethod,params));

    enum  RpcCall{ReturnInvokeMethod = QMetaObject::RemoteInvokeMetaMethod +1, QueryRemoteObject, Construct};

    QREMOTEOBJECT_EXPORT QVariantList getObjectProperties(QObject* object);
    QREMOTEOBJECT_EXPORT QByteArray   getPortableDescription(QObject* object);
    QREMOTEOBJECT_EXPORT QMetaObject getQObjectInstanceFromDescription(QByteArray data, QObject* parent = nullptr);
    QREMOTEOBJECT_EXPORT void setObjectProperties(QObject* object,QVariantList params);
}
#endif // QREMOTEOBJECT_P_H
