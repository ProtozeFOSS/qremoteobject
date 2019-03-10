import QtQuick 2.0

Rectangle{
    color: "transparent"
    id:fob
    property bool keyFobConnected:false
    property string keyID:""
    Image{
        anchors.fill: parent
        fillMode:Image.PreserveAspectFit
        source:"qrc:/keyfob.png"
    }
    Rectangle{
        border.color: "white"
        border.width: 1
        color:"black"
        radius:width
        anchors.top:parent.top
        anchors.topMargin: parent.height*.23
        anchors.left:parent.left
        anchors.leftMargin:parent.width*.22
        height:parent.height*.13
        width:height
        Text{
            anchors.fill: parent
            color:"white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height/2
            text:"L"
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
        border.color: "white"
        border.width: 1
        color:"black"
        radius:width
        anchors.top:parent.top
        anchors.topMargin: parent.height*.23
        anchors.right:parent.right
        anchors.rightMargin:parent.width*.21
        height:parent.height*.13
        width:height
        Text{
            anchors.fill: parent
            color:"white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height/2
            text:"U"
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
}
