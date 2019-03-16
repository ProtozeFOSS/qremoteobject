#include "keyfob.h"

Keyfob::Keyfob(QObject *parent)
    : QRemoteObject(parent), mLockState(false), mLightState(false), mAlarmState(false)
{}

bool Keyfob::alarmState() const
{
    return mAlarmState;
}

bool Keyfob::lockState() const
{
    return mLockState;
}

bool Keyfob::lightState() const
{
    return mLightState;
}


bool Keyfob::trunkState() const
{
    return mTrunkState;
}

void Keyfob::setAlarmState(bool alarm_state)
{
    if(alarm_state != mAlarmState){
        mAlarmState = alarm_state;
        emit alarmChanged(mAlarmState);
    }
}

void Keyfob::setLockState(bool lock_state)
{
    if(lock_state != mLockState){
        mLockState = lock_state;
        emit locksChanged(mLockState);
    }
}

void Keyfob::setLightState(bool light_state)
{
    if(light_state != mLightState){
        mLightState = light_state;
        emit lightsChanged(mLightState);
    }
}

void Keyfob::setTrunkState(bool trunk_state)
{
    if(trunk_state != mTrunkState){
        mTrunkState = trunk_state;
        emit trunkChanged(mTrunkState);
    }
}
