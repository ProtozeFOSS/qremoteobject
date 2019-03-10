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
                lockOutScreen.visible = true;
            }
        }
    }

    InitializeMenu{
        id:initializeMenu
        anchors.fill: parent
        onInitializeSystem:{
            keyfob.keyID = initializeMenu.keyIdInput.text.toUpperCase()
            initializeMenu.enabled = false;
            initializeMenu.visible = false;
            KeyfobObject.connectFob(keyfob.keyID);
        }
    }

    Rectangle{
        id:lockOutScreen
        color: "#e60a0a0a"
        anchors.fill: parent
        visible:false
        Text{
            text:"Invalid Key ID\nSystem Lockout"
            color:"white"
            font.pixelSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

}
