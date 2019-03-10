import QtQuick 2.0

Rectangle {
    color:"transparent"
    Image{
        anchors.fill: parent
        fillMode:Image.PreserveAspectFit
        source:"qrc:/pace-family-overhead-tease.png"
    }
    Connections{
        target:KeyfobInput
        onConnectFob:{
            var index=0;
            for(index; index <ValidKeyfobs.length; ++index){
                if(ValidKeyfobs[index] === key_id){
                    console.log("Keyfob: " + key_id + " Connected!");
                    KeyfobInput.fobConnected(key_id);
                    return;
                }
            }
            KeyfobInput.keyLockedOut(key_id,"Keyfob does not match make and model: (DENIED)");

        }
        onLock:{
            var index=0;
            for(index; index <ValidKeyfobs.length; ++index){
                if(ValidKeyfobs[index] === key_id){
                    console.log("Keyfob: " + key_id + " locked vehicle!");
                    KeyfobInput.setLockState(true);
                }
            }
        }
        onUnlock:{
            var index=0;
            for(index; index <ValidKeyfobs.length; ++index){
                if(ValidKeyfobs[index] === key_id){
                    console.log("Keyfob: " + key_id + " unlocked vehicle!");
                    KeyfobInput.setLockState(false);
                }
            }
        }
    }
}
