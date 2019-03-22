#ifndef KEYFOB_H
#define KEYFOB_H

#include <QObject>
#include "qremoteobject.h"

#ifdef Q_REMOTE_CLIENT
#define KEYFOB_TAG Q_REMOTEONLY
#define CAR_TAG    Q_LOCAL
#else
#define KEYFOB_TAG Q_LOCAL
#define CAR_TAG    Q_REMOTEONLY
#endif

class Keyfob : public QRemoteObject
{
    Q_OBJECT
    Q_REMOTEOBJECT
    Q_PROPERTY(bool lockState READ lockState WRITE setLockState NOTIFY locksChanged MEMBER mLockState)
    Q_PROPERTY(bool lightState READ lightState WRITE setLightState NOTIFY lightsChanged MEMBER mLightState)
    Q_PROPERTY(bool alarmState READ alarmState WRITE setAlarmState NOTIFY alarmChanged MEMBER mAlarmState)
    Q_PROPERTY(bool trunkState READ trunkState WRITE setTrunkState NOTIFY trunkChanged MEMBER mTrunkState)
public:
    explicit Keyfob(QObject *parent = nullptr);
    bool lockState() const;
    bool lightState() const;
    bool alarmState() const;
    bool trunkState() const;

signals:
    KEYFOB_TAG void unlock(QString key_id); // unlock button pressed on keyfob
    KEYFOB_TAG void lock(QString key_id);   // lock button pressed on keyfob
    KEYFOB_TAG void toggleLights(QString key_id); // toggle lights
    KEYFOB_TAG void toggleAlarm(QString key_id);  // toggle alarm
    KEYFOB_TAG void popTrunk(QString key_id);  // toggle alarm
    KEYFOB_TAG void connectFob(QString key_id);

    CAR_TAG    void locksChanged(bool lock_state); // lock state change relayed to keyfobs
    CAR_TAG    void lightsChanged(bool light_state); // light state change relayed to keyfobs
    CAR_TAG    void alarmChanged(bool alarm_state); // alarm state change relayed to keyfobs
    CAR_TAG    void trunkChanged(bool trunk_state); // alarm state change relayed to keyfobs
    CAR_TAG    void keyLockedOut(QString key_id,QString reason); // for flagging unauthorized keyfobs
    CAR_TAG    void fobConnected(QString key_id);

public slots:
    void setLockState(bool lock_state);
    void setLightState(bool light_state);
    void setAlarmState(bool alarm_state);
    void setTrunkState(bool trunk_state);
private:
    bool  mLockState;  // true means locked, false means unlocked
    bool  mLightState; // true means headlights on, false mean headlights off
    bool  mAlarmState; // true means alarm active, false means alarm inactive
    bool  mTrunkState; // true means trunk active, false means alarm inactive

};

#endif // KEYFOB_H
