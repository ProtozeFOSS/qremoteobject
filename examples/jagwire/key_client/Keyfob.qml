import QtQuick 2.0

Rectangle{
    color: "transparent"
    id:fob
    property bool keyFobConnected:false
    property string keyID:""
    property bool doorsLocked:false
    property bool trunkPopped:false
    property bool fogLightsOn:false
    Image{
        id:fobImage
        anchors.fill: parent
        fillMode:Image.PreserveAspectFit
        source:"qrc:/keyfob.png"
        Text{
            id:idText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.topMargin:parent.height*.045
            color:"lightblue"
            text:keyID
            font.family: "Noto Sans Mono CJK JP Bold"
            horizontalAlignment: Text.AlignHCenter
            font.bold:true
            font.pixelSize: parent.height/8
        }
        Rectangle{
            id:lockButton
            anchors.top:parent.top
            anchors.topMargin:parent.height*.21
            color:"#2e2e2e"
            height:fobImage.paintedHeight*.164
            width:height
            anchors.right:parent.horizontalCenter
            anchors.rightMargin:fobImage.paintedWidth*.015
            radius:16
            Image{
                anchors.fill: parent
                anchors.margins: 12
                source:"qrc:/locked.svg"
            }
            Rectangle{
                width:parent.width/4
                height:width
                anchors.top:parent.bottom
                anchors.topMargin: -4
                anchors.horizontalCenter: parent.horizontalCenter
                color: doorsLocked ? "#00a20d":"#3a3a3a"
                radius:width/2
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(keyFobConnected){
                        KeyfobObject.lock(fob.keyID);
                    }
                }
            }
        }
        Rectangle{
            id:unlockButton
            color:"#2e2e2e"
            anchors.top:lockButton.top
            height:lockButton.height
            width:height
            anchors.left:parent.horizontalCenter
            anchors.leftMargin:fobImage.paintedWidth*.026
            radius:16
            Image{
                anchors.fill: parent
                anchors.rightMargin: 10
                anchors.leftMargin:14
                anchors.topMargin:12
                anchors.bottomMargin:12
                source:"qrc:/unlocked.svg"
            }
            Rectangle{
                width:parent.width/4
                height:width
                anchors.top:parent.bottom
                anchors.topMargin: -4
                anchors.horizontalCenter: parent.horizontalCenter
                color: doorsLocked ? "#3a3a3a":"#00a20d"
                radius:width/2
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(keyFobConnected){
                        KeyfobObject.unlock(fob.keyID);
                    }
                }
            }
        }
        Rectangle{
            color:"#2e2e2e"
            anchors.right:parent.horizontalCenter
            anchors.rightMargin:fobImage.paintedWidth*.015
            anchors.top:lockButton.bottom
            anchors.topMargin:parent.height*.05
            height:lockButton.height
            width:height
            radius:16
            Image{
                anchors.fill: parent
                anchors.rightMargin: 10
                anchors.leftMargin:14
                anchors.topMargin:12
                anchors.bottomMargin:12
                source:"qrc:/high-beam.svg"
            }
            Rectangle{
                width:parent.width/4
                height:width
                anchors.top:parent.bottom
                anchors.topMargin: -4
                anchors.horizontalCenter: parent.horizontalCenter
                color: fogLightsOn ? "#00a20d":"#3a3a3a"
                radius:width/2
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(keyFobConnected){
                        KeyfobObject.toggleLights(fob.keyID);
                    }
                }
            }
        }
        Rectangle{
            color:"#2e2e2e"
            anchors.left:parent.horizontalCenter
            anchors.leftMargin:fobImage.paintedWidth*.026
            anchors.top:unlockButton.bottom
            anchors.topMargin:parent.height*.05
            height:lockButton.height
            width:height
            radius:16
            Image{
                anchors.fill: parent
                anchors.rightMargin: 10
                anchors.leftMargin:14
                anchors.topMargin:12
                anchors.bottomMargin:12
                source:"qrc:/trunk.svg"
            }
            Rectangle{
                width:parent.width/4
                height:width
                anchors.top:parent.bottom
                anchors.topMargin: -4
                anchors.horizontalCenter: parent.horizontalCenter
                color: trunkPopped ? "#00a20d":"#3a3a3a"
                radius:width/2
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(keyFobConnected){
                        KeyfobObject.popTrunk(fob.keyID);
                    }
                }
            }
        }
    }
}
