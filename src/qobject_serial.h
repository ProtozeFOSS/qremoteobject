#ifndef QOBJECT_SERIAL_H
#define QOBJECT_SERIAL_H

#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDataStream>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QMetaEnum>
#include <QMetaType>
#include <QMetaClassInfo>
#include <QStringList>
#include <QDebug>
#include <private/qmetaobjectbuilder_p.h>
#include "private/qremoteobject_p.h"

class QRemotePermissions{

public:
    union RemotePermissionDataUnion{
        struct {
            bool Readable       :1;
            bool Resetable      :1;
            bool Toggleable     :1;
            bool Enumtype       :1;
            bool Stored         :1;
            bool Valid          :1;
            bool Writeable      :1;
            bool Localcontrol   :1;
        };
        quint8 permissions :8;
    } data;
    QRemotePermissions(bool readable = false, bool resetable = false,bool toggleable = false,
                    bool enumtype = false, bool stored = false, bool valid = false, bool writeable = false,
                       bool localcontrol = false)
    {
        data.Readable= readable;
        data.Resetable =resetable;
        data.Toggleable =toggleable;
        data.Enumtype = enumtype;
        data.Stored = stored;
        data.Valid = valid;
        data.Writeable = writeable;
        data.Localcontrol = localcontrol;
    }
    QRemotePermissions(const QMetaProperty rhs)
    {
        data.Readable= rhs.isReadable();
        data.Resetable =rhs.isResettable();
        data.Toggleable = !rhs.isFinal();
        data.Enumtype = rhs.isEnumType();
        data.Stored = rhs.isStored();
        data.Valid = rhs.isValid();
        data.Writeable = rhs.isWritable();
        data.Localcontrol = rhs.isScriptable();
    }
};

namespace  QREMOTEOBJECT
{
    Q_NAMESPACE
    Q_CLASSINFO("author", "Christopher Dean")
    Q_CLASSINFO("revision", "0.2")

    constexpr  const char  QRO_NAME[] = "N";    // Name
    constexpr  const char  QRO_DESC[] = "D";    // Description
    constexpr  const char  QRO_TYPE[] = "T";    // Type
    constexpr  const char  QRO_FUNCS[] = "F";    // Funcs Section
    constexpr  const char  QRO_DATA[] = "I";    // Data Section
    constexpr  const char  QRO_PROPS[] = "P";    // Properties Section
    constexpr  const char  QRO_STATE[] = "S";   // state
    constexpr  const char  QRO_PVALUES[] = "V"; // possible values
    constexpr  const char  QRO_ENUMS[] = "E";   //  enums
    constexpr  const char  QRO_CVALUE[] = "v";  // current value
    constexpr  const char  QRO_PERMISS[] = "p";  // Permissions
    constexpr  const char  QRO_INDEX[] = "i";  // Permissions

    QREMOTEOBJECT_EXPORT bool verifyVariantListParams(QVariantList params, QMetaMethod method);

    QREMOTEOBJECT_EXPORT bool generateVoidStarParams(QByteArray data, QMetaMethod prototype, void **& output);
    QREMOTEOBJECT_EXPORT void generateVoidStarVariant(QByteArray data, int type, void**& output);
    QREMOTEOBJECT_EXPORT void generateVoidStarParams(QVariantList& input, void **& output, int ret_type = QMetaType::Void);

    QREMOTEOBJECT_EXPORT QByteArray serializeVoidStar(const QMetaMethod & method, void** args);

    QREMOTEOBJECT_EXPORT QByteArray serializeVoidStarReturn(int t, void* arg);

    QREMOTEOBJECT_EXPORT QByteArray serializeVariantList(const QVariantList & varlist);

    QREMOTEOBJECT_EXPORT QVariantList deserializeVariantList(QByteArray & data);

    QREMOTEOBJECT_EXPORT bool releaseVoidStarParams(const QMetaMethod& method, void **& input);

    QREMOTEOBJECT_EXPORT QByteArray serializeMethod(const QMetaMethod& method);

    QREMOTEOBJECT_EXPORT QMetaMethod deserializeMethod(QByteArray data);

    QREMOTEOBJECT_EXPORT QByteArray serializeProperty(const QMetaProperty& property);

    QREMOTEOBJECT_EXPORT QByteArray serializeEnum(const QMetaEnum& _enum);

    QREMOTEOBJECT_EXPORT QByteArray serializeObject(QObject * parent);

    QREMOTEOBJECT_EXPORT QJsonObject qobjectToJsonReflection(const QObject* obj);

    QREMOTEOBJECT_EXPORT QJsonObject qobjectToRemoteReflection(const QObject* obj);

    QREMOTEOBJECT_EXPORT QMetaMethod  generateMetaMethod(QByteArray signature, QByteArray ret_val = QByteArray("void"),
                                        const QObject * object = nullptr);

    QREMOTEOBJECT_EXPORT QMetaMethod  generateAttachedMetaMethod(const QObject * object,const  QByteArray & data);

    QREMOTEOBJECT_EXPORT QMetaProperty  generateAttachedMetaProperty(QObject * object, QByteArray & data);

    QREMOTEOBJECT_EXPORT QMetaObject* generateMetaObject(QObject * parent, const QByteArray & data);
}
#endif // QOBJECT_SERIAL_GLOBAL_H
