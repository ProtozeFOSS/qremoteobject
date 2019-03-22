import QtQuick 2.0
import QtQuick.Shapes 1.12
Rectangle {
    color:"transparent"
    signal openActionsMenu()
    property bool alarmTransitionState: false
    function closeTrunk(){
        KeyfobInput.setTrunkState(false);
        trunkIcon.color = "#e6ff5555";
        trunkText.text = "Trunk Closed"
    }
    function validateKeyfob(fob_id){
        var index=0;
        var valid=false;
        for(index; index <ValidKeyfobs.length; ++index){
            if(ValidKeyfobs[index] === fob_id){
                valid = true;
                break;
            }
        }
        return valid;
    }

    // define UX for car display
    // rounded shape
    Shape{
        id: dashBackground
        anchors.fill: parent
        anchors.margins: 10
        ShapePath{
            strokeWidth: 2
            strokeColor: "white"
            startX:0;startY:0
            fillGradient: LinearGradient{
                x1:0;y1:0
                x2:0;y2:dashBackground.height
                GradientStop { position: 0; color: "darkgrey" }
                GradientStop { position: .4; color: "#282828" }
                GradientStop { position: 1; color: "black" }
            }

            PathLine{
                x:dashBackground.width;y:0
            }
            PathArc{
                x:0
                y:0
                useLargeArc:true
                radiusY:dashBackground.height
                radiusX:dashBackground.width*.5
            }
        }
        Row{
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.leftMargin: parent.width*.15
            anchors.rightMargin: parent.width*.15
            anchors.topMargin:8
            spacing: parent.width*.2
            Rectangle{
                id:lightIcon
                color: "#e6ff5555"
                border.color: "white"
                border.width: 1
                radius:width/8
                width:parent.width/7
                height:width

                Image{
                    id:lightShape
                    anchors.fill: parent
                    anchors.margins: 8
                    source:"qrc:/high-beam.svg"
                    fillMode: Image.PreserveAspectFit
                }
                Text{
                    id:lightText
                    color:"white"
                    anchors.top:parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: parent.height/4
                    anchors.topMargin: 4
                    horizontalAlignment: Text.AlignHCenter
                    text:"Lights OFF"
                }
            }
            Rectangle{
                id:trunkIcon
                color: "#e6ff5555"
                border.color: "white"
                border.width: 1
                radius:width/8
                width:parent.width/7
                height:width

                Image{
                    id:trunkShape
                    anchors.fill: parent
                    anchors.margins: 8
                    source:"qrc:/trunk.svg"
                    fillMode: Image.PreserveAspectFit
                }
                Text{
                    id:trunkText
                    color:"white"
                    anchors.top:parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: parent.height/4
                    anchors.topMargin: 4
                    horizontalAlignment: Text.AlignHCenter
                    text:"Trunk Closed"
                }
            }
            Rectangle{
                id:lockIcon
                color: "#e6ff5555"
                border.color: "white"
                border.width: 1
                radius:width/8
                width:parent.width/7
                height:width

                Image{
                    id:lockShape
                    anchors.fill: parent
                    anchors.margins: 8
                    source:"qrc:/unlocked.svg"
                    fillMode: Image.PreserveAspectFit
                }
                Text{
                    id:lockText
                    color:"white"
                    anchors.top:parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: parent.height/4
                    anchors.topMargin: 4
                    horizontalAlignment: Text.AlignHCenter
                    text:"Doors Unlocked"
                }
            }
        }
        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*.018
            height:parent.height/6
            radius:16
            width:parent.width/6
            color: "#006aff"
            border.width: 1
            border.color:"white"
            Text{
                anchors.fill: parent
                text: "Actions"
                color:"white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    openActionsMenu()
                }
            }
        }
        Rectangle{
          id:alarmNotifier
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.verticalCenter: parent.verticalCenter
          anchors.verticalCenterOffset: parent.height * .15
          width:parent.width/4
          height:width/3
          radius:height/2
          color:"red"
          opacity: 0
          Text{
              anchors.fill: parent
              anchors.margins: parent.radius
              text:"Alarm Active"
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
              color:"white"
              visible:parent.opacity >= 0.5
          }
          Behavior on opacity{
              NumberAnimation{
                  duration: 1000
              }
          }
        }
    }
    Timer{
        id:alarmTimer
        interval:1000
        repeat:true
        running:false
        onTriggered: {
            KeyfobInput.setLightState(!KeyfobInput.lightState)
            if(KeyfobInput.lightState){
                lightIcon.color = "#00ddff"
                lightText.text = "Lights ON"
            }else{
                lightIcon.color = "#e6ff5555"
                lightText.text = "Lights OFF"
            }
            alarmTransitionState = !alarmTransitionState;
            if(alarmTransitionState){
                alarmNotifier.opacity = .2;
            }else{
                alarmNotifier.opacity = 1.0;
            }
        }
        onRunningChanged: {
            if(!running){
                alarmNotifier.opacity = 0;
                KeyfobInput.setLightState(false)
                lightIcon.color = "#e6ff5555"
                lightText.text = "Lights OFF"
            }
        }
    }

    Connections{
        target:KeyfobInput
        onConnectFob:{
            if(validateKeyfob(key_id)){
                console.log("Keyfob: " + key_id + " Connected!");
                KeyfobInput.fobConnected(key_id);
            }else{
                KeyfobInput.keyLockedOut(key_id,"Keyfob does not match make and model: (DENIED)");
            }

        }
        onLock:{
            if(validateKeyfob(key_id)){
                console.log("Keyfob: " + key_id + " locked vehicle!");
                KeyfobInput.setLockState(true);
                lockShape.source = "qrc:/locked.svg"
                lockIcon.color = "#00ddff"
                lockText.text = "Doors Locked"
            }
        }
        onUnlock:{
            if(validateKeyfob(key_id)){
                console.log("Keyfob: " + key_id + " unlocked vehicle!");
                KeyfobInput.setLockState(false);
                lockShape.source = "qrc:/unlocked.svg"
                lockIcon.color = "#e6ff5555"
                lockText.text = "Doors Unlocked"
            }
        }
        onPopTrunk:{
            if(validateKeyfob(key_id)){
                console.log("Keyfob: " + key_id + " popping trunk!");
                trunkIcon.color = "#00ddff"
                trunkText.text = "Trunk Open"
                KeyfobInput.setTrunkState(true)
            }
        }
        onToggleLights:{
            if(validateKeyfob(key_id)){
                KeyfobInput.setLightState(!KeyfobInput.lightState)
                if(KeyfobInput.lightState){
                    console.log("Keyfob: " + key_id + " toggle lights ON!");
                    lightIcon.color = "#00ddff"
                    lightText.text = "Lights ON"
                }else{
                    console.log("Keyfob: " + key_id + " toggle lights OFF!");
                    lightIcon.color = "#e6ff5555"
                    lightText.text = "Lights OFF"
                }
            }
        }
        onToggleAlarm:{
            if(validateKeyfob(key_id)){
                KeyfobInput.setAlarmState(!KeyfobInput.alarmState)
                if(KeyfobInput.alarmState){
                    console.log("Keyfob: " + key_id + " Activated Alarm!");
                    alarmTimer.running = true;
                }else{
                    console.log("Keyfob: " + key_id + " Deactivated Alarm!");
                    alarmTimer.running = false;
                }
            }
        }
    }
}
