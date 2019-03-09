#ifndef QREMOTESERVICE_P_H
#define QREMOTESERVICE_P_H
#include <QSharedData>
#include "../qremoteobject.h"

/*****************************************************************
 * Stores the definition of an explicitly shared data object for
 * RemoteService. This will allow apis, containers, and the like
 * to pass QRemoteService throughout the system with low copy
 * overhead. This also allows multiple references to the same
 * QRemoteService (often needed on the server and client side).
 * ***************************************************************/


namespace ServiceActions{ // eases the creation of action types
class ActionList;
    typedef enum class ActionType{
        FUNCTION = 0,
        READ,
        WRITE,
        RETURN,
        RETURN_READ
    }ActionType;



    class ServiceAction{
        friend class ActionList;
    public:
        ServiceAction(int action_id = -1):
            type(ActionType::FUNCTION), id(action_id), parameters(), return_id(-1){}
        ServiceAction& operator =(const ServiceAction& rhs)
        {
            type = rhs.type;
            id = rhs.id;
            parameters = rhs.parameters;
            return_id = rhs.return_id;
            return (*this);
        }
        ServiceAction(const ServiceAction & rhs)
        {
            *this = rhs;
        }
        ServiceActions::ActionType getType()
        {
            return type;
        }
        int  id;
        QVariantList  parameters;
        int             return_id;

    protected:
        ServiceActions::ActionType type;
    };

    class ActionListPrivate : public QSharedData
    {
    public:
        QList<ServiceAction>  mActions; // this list is executed 0->max_length

    };


    class ActionList{

    public:
        ActionList(){ mData = new ActionListPrivate();}
        ActionList(const ActionList& rhs) : mData(rhs.mData)
        {

        }
        void appendReadProperty(int id, int return_addr)
        {
            ServiceAction action(id);
            action.type = ActionType::READ;
            action.return_id = return_addr;
            mData->mActions.append(action);
        }
        void appendSetProperty(int id, QVariant value)
        {
            ServiceAction action(id);
            action.type = ActionType::WRITE;
            action.parameters.append(value);
            mData->mActions.append(action);
        }
        void appendFunction(int id, QVariantList& params, int return_addr)
        {
            ServiceAction action(id);
            action.type = ActionType::FUNCTION;
            action.parameters =params;
            action.return_id = return_addr;
            mData->mActions.append(action);
        }
        void appendReturnFunction(int id, QVariant  ret_val, int return_addr)
        {
            ServiceAction action(id);
            action.type = ActionType::RETURN;
            action.parameters.append(ret_val);
            action.return_id = return_addr;
            mData->mActions.append(action);
        }
        void appendReturnRead(int id, QVariant  ret_val, int return_addr)
        {
            ServiceAction action(id);
            action.type = ActionType::RETURN_READ;
            action.parameters.append(ret_val);
            action.return_id = return_addr;
            mData->mActions.append(action);
        }
        void appendAction(const ServiceAction& action)
        {
            mData->mActions.append(action);
        }
        ServiceAction popLast()
        {
            if(mData->mActions.length() > 0)
                return mData->mActions.takeLast();
            return ServiceAction();
        }

        ServiceAction popFront()
        {
            if(mData->mActions.length() > 0)
                return mData->mActions.takeFirst();
            return ServiceAction();
        }

        int actionCount() const
        {
            return mData->mActions.count();
        }
        QByteArray toBinary()
        {
            QJsonArray  actions;
            foreach(ServiceAction action, mData->mActions)
            {
                QJsonArray array;
                array.append(action.id);
                array.append((int)action.type);
                array.append(action.return_id);
                foreach(QVariant var, action.parameters)  // returns will only have 1 parameter
                {
                    array.append(QJsonValue::fromVariant(var));
                }
                actions.append(array);
            }
            QJsonDocument doc;
            doc.setArray(actions);
            return doc.toBinaryData();
        }
        void fromBinary(QByteArray bin)
        {
            mData->mActions.clear();
            QJsonDocument doc = QJsonDocument::fromBinaryData(bin);
            QJsonArray array = doc.array();
            int count = array.count();
            for(int index(0); index < count; index++)
            {
                QJsonArray action = array.at(index).toArray();
                ServiceAction service_action(action.at(0).toInt());
                service_action.type = ServiceActions::ActionType(action.at(1).toInt());
                service_action.return_id =  action.at(2).toInt();
                int action_count = action.count();
                for(int pindex(3); pindex < action_count;  ++pindex)
                {
                    service_action.parameters.append(action.at(pindex));
                }
                mData->mActions.append(service_action);
            }
        }

    protected:
        QExplicitlySharedDataPointer<ActionListPrivate> mData;
    };
}

#include <QSharedData>
#include <QSize>
#include <QString>
#include <QPoint>
class QRemoteObject;
#include <QList>
#include "../qobject_serial.h"

class  QRemoteServicePrivate : public QSharedData
{

public:
    QRemoteServicePrivate() : QSharedData(), objects(),directory(){}
    QRemoteServicePrivate(const QRemoteServicePrivate& rhs)
        : QSharedData(), domain(rhs.domain), objects(rhs.objects), directory(rhs.directory){}

    void addToDirectory(QJsonObject obj)
    {
        QString name = obj.value(QREMOTEOBJECT::QRO_NAME).toString();
        if(!name.isEmpty()){
            directory.insert(name, obj);
        }
    }
    QJsonObject retrieveFromDirectory(QString name) const
    {
        QJsonObject ret_val;
        if(!name.isEmpty()){
            ret_val = directory.value(name).toObject();
        }
        return ret_val;
    }
    void removeFromDirectory(QString name)
    {
        if(!name.isEmpty()){
            directory.remove(name);
        }
    }
    // remote service wont own these objects
    QList<QRemoteObject*>  objects;
    QJsonObject   directory;
    QString domain;
    QString description;
    QString type;
};


#endif // QREMOTESERVICE_P_H
