import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id:window
    visible: true
    property bool resizedHeight: false
    property bool resizedWidth: false
    width: 224
    height: 366
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
                KeyfobObject.initializeRemote();
                console.log("Keyfob " + keyfob.keyID + " initialized and ready")
            }
        }
        onLocksChanged:{
            keyfob.doorsLocked = lock_state;
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
        onLightsChanged:{
            keyfob.fogLightsOn = light_state;
            if(light_state === true){
                console.log("Recieved confirmation car is locked")
            }else{
                console.log("Recieved confirmation car is unlocked")
            }
        }
        onTrunkChanged:{
            keyfob.trunkPopped = trunk_state;
            if(trunk_state === true){
                console.log("Recieved confirmation trunk popped")
            }else{
                console.log("Recieved confirmation trunk closed")
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

    // resizing (jitter)
    onWidthChanged: {
        if(!resizedWidth){
            resizeFob.start();
        }
    }
    onHeightChanged: {
       if(!resizedHeight){
           resizeFob.start();
       }
    }
    Timer{
        id:doneResizing
        repeat: false
        interval:10
        onTriggered:{
            resizedHeight = false;
            resizedWidth = false;
        }
    }
    Timer{
        id:resizeFob
        repeat: false
        interval:1
        onTriggered:{
                resizedHeight = true;
                resizedWidth = true;
                window.height = window.width*1.6339291
                window.width = window.height*0.61202191
                doneResizing.start();

        }
    }
}
