import QtQuick 2.0
import QtQuick.Shapes 1.12
Rectangle {
    color:"transparent"
    signal openActionsMenu()
    function closeTrunk(){
        KeyfobInput.setTrunkState(false);
        trunkIcon.color = "#e6ff5555";
        trunkText.text = "Trunk Closed"
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
            anchors.bottomMargin: 4
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
                    lockShape.source = "qrc:/locked.svg"
                    lockIcon.color = "#00ddff"
                    lockText.text = "Doors Locked"
                }
            }
        }
        onUnlock:{
            var index=0;
            for(index; index <ValidKeyfobs.length; ++index){
                if(ValidKeyfobs[index] === key_id){
                    console.log("Keyfob: " + key_id + " unlocked vehicle!");
                    KeyfobInput.setLockState(false);
                    lockShape.source = "qrc:/unlocked.svg"
                    lockIcon.color = "#e6ff5555"
                    lockText.text = "Doors Unlocked"
                }
            }
        }
        onPopTrunk:{
            var index=0;
            for(index; index <ValidKeyfobs.length; ++index){
                if(ValidKeyfobs[index] === key_id){
                    console.log("Keyfob: " + key_id + " popping trunk!");
                    trunkIcon.color = "#00ddff"
                    trunkText.text = "Trunk Open"
                    KeyfobInput.setTrunkState(true)
                }
            }
        }
        onToggleLights:{
            var index=0;
            for(index; index <ValidKeyfobs.length; ++index){
                if(ValidKeyfobs[index] === key_id){
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
        }
    }
}
