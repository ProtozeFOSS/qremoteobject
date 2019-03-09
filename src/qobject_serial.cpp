#include "qobject_serial.h"
#include <QByteArray>
bool QREMOTEOBJECT::verifyVariantListParams(QVariantList params, QMetaMethod method)
{
    int p_count = params.count();
    if(method.parameterCount() > p_count) // invalid parameter size
    {
        return false;
    }

    for(int index(0); index < method.parameterCount(); index++)
    {
        QVariant param = params.at(1+index);
        int my_type = method.parameterType(index);
        if(!param.canConvert(my_type))
        {
            return false; // param doesnt match
        }
    }
    return true;
}


bool QREMOTEOBJECT::generateVoidStarParams(QByteArray data, QMetaMethod prototype, void **& output)
{
    // determine the number of arguments
    int arg_count(prototype.parameterCount());
    void** out_data = new void*[arg_count+1];
    QDataStream ss(&data,QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_10);
    bool success(true);
    int type(-1);
    int index(0);
    uint length(0);
    void * param = nullptr;
    out_data[0] = QMetaType::create(prototype.returnType());
    for(; index < arg_count; ++index){
        type = prototype.parameterType(index);
        void*& current_param = out_data[index+1];
        switch(type){
            case QMetaType::QByteArray:{
                length = 0;
                if(!QMetaType::load(ss,QMetaType::Int,&length))// if length didnt load
                {
                    // unwind
                    goto UNWIND;
                }
                current_param = QMetaType::create(type = prototype.parameterType(index));
                if(length > 0){
                    char * buffer = new char[length+1];
                    buffer[length] = '\0';
                    ss.readRawData(buffer,length);
                    static_cast<QByteArray*>(current_param)->setRawData(buffer,length);
                    //delete [] buffer;
                }
                break;
            }
            case QMetaType::QJsonObject:{
                length = 0;
                if(!QMetaType::load(ss,QMetaType::Int,&length))// if length didnt load
                {
                    // unwind
                    goto UNWIND;
                }
                QByteArray *data(nullptr);
                current_param = QMetaType::create(QMetaType::QByteArray);
                if(length > 0){
                    char * buffer = new char[length+1];
                    buffer[length] = '\0';
                    ss.readRawData(buffer,length);
                    data = static_cast<QByteArray*>(current_param);
                    data->setRawData(buffer,length);
                    //delete [] buffer;
                }
                QJsonDocument doc = QJsonDocument::fromBinaryData(*data);
                data->clear();
                data->detach();
                QMetaType::destroy(QMetaType::QByteArray,current_param);
                if(doc.isObject()){
                    QJsonObject param = doc.object();
                    current_param = QMetaType::create(QMetaType::QJsonObject);
                    QJsonObject* param_ptr = static_cast<QJsonObject*>(current_param);
                    *param_ptr = param;
                    //current_param = &param;
                } else{
                    goto UNWIND;
                }
                break;
            }
            default:{
                current_param = QMetaType::create(type);
                if(!QMetaType::load(ss,type,current_param))
                {
                    goto UNWIND;
                }
                break;
            }
        }
    }
    output = out_data;
    goto FINISHED; // success

    UNWIND: // not so much
    success = false;
    while(index > 0){
        param = out_data[--index];
        type = prototype.parameterType(index);
        switch(type){
            case QMetaType::QByteArray:{
                --index;
                QByteArray* array = static_cast<QByteArray*>(param);
                array->clear();
                //param = QMetaType::create(type = prototype.parameterType(index));
                QMetaType::destroy(type,param);
                break;
            }
            default:{
                QMetaType::destroy(type,param);
                break;
            }
        }
    }
    QMetaType::destroy(prototype.returnType(),out_data[0]);
    delete [] out_data;
    output = nullptr;
    qDebug() << "Failed to load parameter of type: " << QMetaType::typeName(type);
    qDebug() << "In function : " << prototype.methodSignature();

    FINISHED:
    return success;
}

void QREMOTEOBJECT::generateVoidStarParams(QVariantList& input, void **& output, int ret_type)
{
    // determine the number of arguments
    int arg_count = input.length();
    void** out_data = new void*[arg_count+1];
    if(ret_type != QMetaType::Void){
        out_data[0] = QMetaType::create(int(ret_type));
    }
    if(out_data == nullptr){
        // failed to allocate memory?
        return;
    }
    for(int index(0); index < arg_count; index++){
        QVariant data = input.at(index);
        out_data[index+1] = QMetaType::create(int(data.type()),data.data());// zero is ret val
    }
    output = out_data;
}


void QREMOTEOBJECT::generateVoidStarVariant(QByteArray data, int type, void **&output)
{
    void ** value = new void*[1];
    QDataStream ss(&data,QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_10);
    switch(type){
        case QMetaType::QByteArray:{
            int length(-1);
            QMetaType::load(ss,QMetaType::Int,&length);
            if(length > 0){
                value[0] = QMetaType::create(type);
                char * buffer = new char[length+1];
                buffer[length] = '\0';
                ss.readRawData(buffer,length);
                static_cast<QByteArray*>(value[0])->setRawData(buffer,length);
            }
            break;
        }
        default:{
            value[0] = QMetaType::create(type);
            QMetaType::load(ss,type,value[0]);
            break;
        }
    }
    output = value;
}


QByteArray QREMOTEOBJECT::serializeVoidStarReturn(int t, void* arg)
{
    QByteArray bdata;
    QDataStream ss(&bdata,QIODevice::WriteOnly);
    switch(t){
        case QMetaType::QByteArray:{
        QByteArray* array = static_cast<QByteArray*>(arg);
            ss << *array;
            break;
        }
        case QMetaType::QJsonObject:{
            QJsonObject* obj = static_cast<QJsonObject*>(arg);
            QJsonDocument doc;
            doc.setObject(*obj);
            QByteArray data = doc.toBinaryData();
            ss << data;
            break;
        }
        default: QMetaType::save(ss,t,arg);
    }
    return bdata;
}


QByteArray QREMOTEOBJECT::serializeVariantList(const QVariantList & varlist)
{
    int count = varlist.count();
    // assumed first value is return value
    // begin the massive conversion by type
    QByteArray bdata;
    QDataStream ss(&bdata,QIODevice::WriteOnly);
    ss << count; // count in
    for(int pindex(0); pindex < count; pindex++){
        int type(varlist.at(pindex).type());
        ss << type;
        QMetaType::save(ss,type, varlist.at(pindex).data());
    }
    return bdata;
}

QVariantList QREMOTEOBJECT::deserializeVariantList(QByteArray & data)
{
    QDataStream ss(&data,QIODevice::ReadOnly);
    int count(-1);
    ss >> count;
    int type(-1);
    QVariantList ret_val;
    for(int pindex(0); pindex < count; pindex++){
        ss >> type;
        QVariant var;
        if(!QMetaType::load(ss,type,var.data())){
            qDebug() << "Failed to deserialize variant @ " << pindex;
        }
        else{
            ret_val.append(var);
        }
    }
    return ret_val;
}

bool QREMOTEOBJECT::releaseVoidStarParams(const QMetaMethod& method, void **& input)
{
    // determine the number of arguments
    int release_count(0);
    int arg_count = method.parameterCount();
    for(int index(0); index < arg_count; ++index){
        QMetaType::destroy(method.parameterType(index),input[index+1]);
        release_count += 1;
        input[index+1] = nullptr;
    }
    QMetaType::destroy(method.returnType(),input[0]);
    delete [] input;
    input = nullptr;
    return (release_count == arg_count);
}

QByteArray QREMOTEOBJECT::serializeMethod(const QMetaMethod& method)
{
    QMetaObjectBuilder b;
    b.addMethod(method);
    return b.toRelocatableData();
}

QMetaMethod QREMOTEOBJECT::deserializeMethod(QByteArray data)
{
    QMetaObjectBuilder b;
    QMetaObject obj;
    QMetaObject empty;
    b.fromRelocatableData(&obj,&empty,data);
    return obj.method(0);
}

QByteArray QREMOTEOBJECT::serializeProperty(const QMetaProperty& property)
{
    QMetaObjectBuilder b;
    b.addProperty(property);
    return b.toRelocatableData();
}

QByteArray QREMOTEOBJECT::serializeEnum(const QMetaEnum& _enum)
{
    QMetaObjectBuilder b;
    b.addEnumerator(_enum);
    return b.toRelocatableData();
}

QByteArray QREMOTEOBJECT::serializeObject(QObject * parent)
{
    QMetaObjectBuilder  b;
    const QMetaObject *obj_m = parent->metaObject();
    int methods = obj_m->methodCount();
    int properties = obj_m->propertyCount();
    int enums = obj_m->enumeratorCount();
    int infos = obj_m->classInfoCount();
    b.setClassName(obj_m->className());
    for(int iindex(obj_m->classInfoOffset()); iindex < infos; iindex++)
    {
        QMetaClassInfo info = obj_m->classInfo(iindex);
        b.addClassInfo(info.name(),info.value());
    }
    for(int mindex(obj_m->methodOffset()); mindex<methods; mindex++)
    {
        b.addMethod(obj_m->method(mindex));
    }
    for(int pindex(obj_m->propertyOffset()); pindex<properties; pindex++)
    {
        b.addProperty(obj_m->property(pindex));
    }
    for(int eindex(obj_m->enumeratorOffset()); eindex<enums; eindex++)
    {
        b.addEnumerator(obj_m->enumerator(eindex));
    }
    for(int cindex(0); cindex < obj_m->constructorCount(); cindex++)
    {
        b.addConstructor(obj_m->constructor(cindex));
    }
    return b.toRelocatableData();
}

QMetaMethod  QREMOTEOBJECT::generateMetaMethod(QByteArray signature, QByteArray ret_val ,
                                        const QObject * object )
{
    QMetaObjectBuilder b;
    if(object)
    {
        b.setSuperClass(object->metaObject());
    }
    b.addMethod(signature,ret_val);
    QMetaObject *obj = b.toMetaObject();
    return obj->method(0);
}
QMetaMethod  QREMOTEOBJECT::generateAttachedMetaMethod(const QObject * object,const  QByteArray & data)
{
    QMetaObjectBuilder b;
    QMetaObject  meta_obj;
    b.fromRelocatableData(&meta_obj,object->metaObject(),data);
    return meta_obj.method(0);
}
QMetaProperty  QREMOTEOBJECT::generateAttachedMetaProperty(QObject * object, QByteArray & data)
{
    QMetaObjectBuilder b;
    QMetaObject  meta_obj;
    b.fromRelocatableData(&meta_obj,object->metaObject(),data);
    return meta_obj.property(0);
}
QMetaObject* QREMOTEOBJECT::generateMetaObject(QObject * parent, const QByteArray & data)
{
    QMetaObject * obj = new QMetaObject();
    QMetaObjectBuilder  b;
    if(parent)
    {
        b.fromRelocatableData(obj,parent->metaObject(),data);
    }
    else
    {
        QObject q;
        const QMetaObject* qmeta = q.metaObject();
        b.fromRelocatableData(obj,qmeta,data);
    }
    return obj;
}


QJsonObject QREMOTEOBJECT::qobjectToRemoteReflection(const QObject *obj)
{
    QJsonObject root_object;
    if(obj){
        const QMetaObject*  meta(obj->metaObject());
        int enum_count = meta->enumeratorCount();
        QJsonArray enums;
        // ENUMs
        for(int edex(meta->enumeratorOffset()); edex < enum_count; edex++)
        {
            QMetaEnum meta_enum(meta->enumerator(edex));
            QJsonObject enum_obj;
            enum_obj[QREMOTEOBJECT::QRO_NAME] = meta_enum.name();
            enum_obj[QREMOTEOBJECT::QRO_TYPE] = meta_enum.scope();
            enum_obj[QREMOTEOBJECT::QRO_STATE] = meta_enum.isFlag();
            int key_count = meta_enum.keyCount();
            QJsonArray  keys;
            for(int kdex(0); kdex<key_count; kdex++)
            {
                keys.append(meta_enum.key(kdex));
                keys.append(meta_enum.value(kdex));
            }
            enum_obj[QREMOTEOBJECT::QRO_PVALUES] = keys;
            enums.append(enum_obj);
        }
        root_object[QREMOTEOBJECT::QRO_ENUMS] = enums;
        QJsonArray   properties;
        //Properties
        int property_count = meta->propertyCount();
        for(int pdex(meta->propertyOffset()); pdex < property_count; pdex++)
        {
            QMetaProperty  meta_prop(meta->property(pdex));
            QJsonObject property;
            if(meta_prop.isUser()){
                property[QREMOTEOBJECT::QRO_NAME] = meta_prop.name();
                property[QREMOTEOBJECT::QRO_TYPE] = int(meta_prop.type());
                QRemotePermissions  permission(meta_prop);
                property[QREMOTEOBJECT::QRO_PERMISS] = int(permission.data.permissions);
                property[QREMOTEOBJECT::QRO_CVALUE]  = QJsonValue::fromVariant(meta_prop.read(obj));
                property[QREMOTEOBJECT::QRO_INDEX] = pdex;
                property[QREMOTEOBJECT::QRO_STATE] = meta_prop.revision();
                properties.append(property);
            }
        }

        // Funcs
        QJsonArray funcs;
        int func_count = meta->methodCount();
        for(int fdex(meta->methodOffset()); fdex < func_count; fdex++)
        {
            QJsonObject method_obj;
            QMetaMethod   method(meta->method(fdex));
            if((QStringLiteral("Q_REMOTE").compare(method.tag()) ==0 || QStringLiteral("Q_REMOTEONLY").compare(method.tag()) == 0)
                    && method.access() == QMetaMethod::Public){
                method_obj[QREMOTEOBJECT::QRO_NAME] = QString::fromLocal8Bit(method.methodSignature());
                method_obj[QREMOTEOBJECT::QRO_TYPE] = int(method.methodType());
                method_obj[QREMOTEOBJECT::QRO_INDEX] = fdex;
                method_obj[QREMOTEOBJECT::QRO_PERMISS] = int(method.access());
                QJsonArray params;
                int parameter_count = method.parameterCount();
                QList<QByteArray> param_names(method.parameterNames());
                for(int paramdex(0); paramdex < parameter_count; paramdex++)
                {
                    params.append(int(method.parameterType(paramdex)));
                    params.append(QString::fromLocal8Bit(param_names.at(paramdex)));
                }
                method_obj[QREMOTEOBJECT::QRO_PVALUES] = params;
                method_obj[QREMOTEOBJECT::QRO_DATA] = int(method.returnType());
                funcs.append(method_obj);
            }
        }
        root_object[QREMOTEOBJECT::QRO_NAME] = obj->objectName();
        root_object[QREMOTEOBJECT::QRO_ENUMS] = enums;
        root_object[QREMOTEOBJECT::QRO_FUNCS] = funcs;
        root_object[QREMOTEOBJECT::QRO_PROPS] = properties;
    }
    else{
        root_object.insert("error",true);
    }
    return root_object;
}

QJsonObject QREMOTEOBJECT::qobjectToJsonReflection(const QObject *obj)
{
    QJsonObject root_obj;
    if(obj){
        const QMetaObject*  meta(obj->metaObject());
        int enum_count = meta->enumeratorCount();
        QJsonArray enums;
        // ENUMs
        for(int edex(meta->enumeratorOffset()); edex < enum_count; edex++)
        {
            QMetaEnum meta_enum(meta->enumerator(edex));
            QJsonObject enum_obj;
            enum_obj[QREMOTEOBJECT::QRO_NAME] = meta_enum.name();
            enum_obj[QREMOTEOBJECT::QRO_TYPE] = meta_enum.scope();
            enum_obj[QREMOTEOBJECT::QRO_STATE] = meta_enum.isFlag();
            int key_count = meta_enum.keyCount();
            QJsonArray  keys;
            for(int kdex(0); kdex<key_count; kdex++)
            {
                keys.append(meta_enum.key(kdex));
                keys.append(meta_enum.value(kdex));
            }
            enum_obj[QREMOTEOBJECT::QRO_PVALUES] = keys;
            enums.append(enum_obj);
        }
        root_obj[QREMOTEOBJECT::QRO_ENUMS] = enums;
        QJsonArray   properties;
        //Properties
        int property_count = meta->propertyCount();
        for(int pdex(meta->propertyOffset()); pdex < property_count; pdex++)
        {
            QMetaProperty  meta_prop(meta->property(pdex));
            QJsonObject property;
            property[QREMOTEOBJECT::QRO_NAME] = meta_prop.name();
            property[QREMOTEOBJECT::QRO_TYPE] = int(meta_prop.type());
            QRemotePermissions  permission(meta_prop);
            property[QREMOTEOBJECT::QRO_PERMISS] = int(permission.data.permissions);
            property[QREMOTEOBJECT::QRO_CVALUE]  = QJsonValue::fromVariant(meta_prop.read(obj));
            property[QREMOTEOBJECT::QRO_INDEX] = pdex;
            property[QREMOTEOBJECT::QRO_STATE] = meta_prop.revision();
            properties.append(property);
        }

        // Funcs
        QJsonArray funcs;
        int func_count = meta->methodCount();
        for(int fdex(meta->methodOffset()); fdex < func_count; fdex++)
        {
            QJsonObject method_obj;
            QMetaMethod   method(meta->method(fdex));
            method_obj[QREMOTEOBJECT::QRO_NAME] = QString::fromLocal8Bit(method.methodSignature());
            method_obj[QREMOTEOBJECT::QRO_TYPE] = int(method.methodType());
            method_obj[QREMOTEOBJECT::QRO_INDEX] = fdex;
            method_obj[QREMOTEOBJECT::QRO_PERMISS] = int(method.access());
            QJsonArray params;
            int parameter_count = method.parameterCount();
            QList<QByteArray> param_names(method.parameterNames());
            for(int paramdex(0); paramdex < parameter_count; paramdex++)
            {
                params.append(int(method.parameterType(paramdex)));
                params.append(QString::fromLocal8Bit(param_names.at(paramdex)));
            }
            method_obj[QREMOTEOBJECT::QRO_PVALUES] = params;
            method_obj[QREMOTEOBJECT::QRO_DATA] = int(method.returnType());
            funcs.append(method_obj);
        }
        root_obj[QREMOTEOBJECT::QRO_NAME] = obj->objectName();
        root_obj[QREMOTEOBJECT::QRO_ENUMS] = enums;
        root_obj[QREMOTEOBJECT::QRO_FUNCS] = funcs;
        root_obj[QREMOTEOBJECT::QRO_PROPS] = properties;
    }
    else{
        root_obj.insert("error",true);
    }
    return root_obj;
}
