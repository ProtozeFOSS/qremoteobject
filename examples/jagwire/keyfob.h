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
public:
    enum LockedState{ UNLOCKED = 0, LOCKED = 1};
    Q_ENUM(LockedState)

    explicit Keyfob(QObject *parent = nullptr);
    bool lockState() const;
    bool lightState() const;
    bool alarmState() const;

signals:
    KEYFOB_TAG void unlock(QString key_id); // unlock button pressed on keyfob
    KEYFOB_TAG void lock(QString key_id);   // lock button pressed on keyfob
    KEYFOB_TAG void toggleLights(QString key_id); // toggle lights
    KEYFOB_TAG void toggleAlarm(QString key_id);  // toggle alarm

    CAR_TAG    void locksChanged(bool lock_state); // lock state change relayed to keyfobs
    CAR_TAG    void lightsChanged(bool light_state); // light state change relayed to keyfobs
    CAR_TAG    void alarmChanged(bool light_state); // alarm state change relayed to keyfobs
    CAR_TAG    void keyLockedOut(QString reason); // for flagging unauthorized keyfobs

public slots:
    void setLockState(bool lock_state);
    void setLightState(bool light_state);
    void setAlarmState(bool alarm_state);
private:
    bool  mLockState;  // true means locked, false means unlocked
    bool  mLightState; // true means headlights on, false mean headlights off
    bool  mAlarmState; // true means alarm active, false means alarm inactive
};

#endif // KEYFOB_H
