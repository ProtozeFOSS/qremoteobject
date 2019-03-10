import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id:window
    visible: true
    width: 448
    height: 732
    title: qsTr("Hello Keyfob")
    color:"#FF282828"
    Keyfob{
        id:keyfob
        anchors.fill: parent
        keyID:"B2C3"
    }
    Connections{
        target:CarConnection
        onConnected:{
            console.log("Connected to car server");
            KeyfobObject.connectFob(keyfob.keyID);
        }

    }
    Connections{
        target:KeyfobObject
        onFobConnected:{
            if(keyfob.keyID == key_id){
                keyfob.keyFobConnected = true;
                console.log("Keyfob " + keyfob.keyID + " initialized and ready")
            }
        }
        onLocksChanged:{
            if(lock_state === true){
                console.log("Recieved confirmation car is locked")
            }else{
                console.log("Recieved confirmation car is unlocked")
            }
        }
        onKeyLockedOut:{
            if(keyfob.keyID == key_id){
                console.log("Keyfob locked out for reason: " + reason)
            }
        }
    }

}
