import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
Window {
    id:topWindow
    visible: true
    x:500
    y:200
    width: 546
    height: 243
    title: qsTr("Hello Car")
    color:"black"
    CarDash{
        id:carDash
        anchors.fill: parent
        onOpenActionsMenu: {
            actionsMenu.visible = true
            actionsMenu.y = 0;
        }
    }
    Rectangle{
        id:actionsMenu
        color:"lightgrey"
        width:parent.width
        height:parent.height
        y:parent.height
        visible:false
        Behavior on y{
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
                alwaysRunToEnd: true
            }
        }
        Row{
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.bottom:closeButton.top
            anchors.right:parent.right
            anchors.margins:10
            spacing:20
            Column{
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    height:24
                    font.bold:true
                    text:"Valid Key Fobs"
                    font.pixelSize: 24
                    width:parent.width
                }
                height:parent.height
                width:parent.width/2 - 20

                spacing:10
                ListView{
                    id:fobView
                    clip:true
                    anchors.horizontalCenter: parent.horizontalCenter
                    height:parent.height - 40
                    width:parent.width/2
                    model:ValidKeyfobs
                    spacing:2
                    ScrollBar.vertical: ScrollBar { }
                    delegate:Rectangle{
                        height:40
                        width:fobView.width
                        color:"#2d2d2d"
                        border.width: 1
                        border.color: "white"
                        Text{
                            anchors.fill: parent
                            color:"white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: modelData
                            font.pixelSize:28
                        }
                    }
                }
            }
            Column{
                height:parent.height
                width:parent.width/2 - 20
                spacing:10
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    height:24
                    font.bold:true
                    text:"Actions"
                    font.pixelSize: 24
                    width:parent.width
                }
                Rectangle{
                    color:"#474747"
                    height:parent.height - 40
                    width:parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: "white"
                    border.width: 1
                    radius:4
                    Column{
                        anchors.fill: parent
                        anchors.margins: 8
                        Rectangle{
                            height:40
                            width:parent.width*.8
                            color:"blue"
                            border.color: "white"
                            border.width: 1
                            anchors.horizontalCenter: parent.horizontalCenter
                            radius:8
                            Text{
                                anchors.fill: parent
                                text:"Close Trunk Lid"
                                color:'white'
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            MouseArea{
                                anchors.fill: parent
                                onClicked:{
                                    carDash.closeTrunk();
                                }
                            }
                        }
                    }
                }
            }

        }
        Rectangle{
            id:closeButton
            height:48
            width:height
            radius:width/2
            color:"red"
            border.color: "white"
            border.width: 1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom:parent.bottom
            anchors.bottomMargin: 8
            Text{
                anchors.fill: parent
                text:"X"
                color:"white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: parent.width/2
            }
            MouseArea{
                anchors.fill: parent
                onClicked:{
                    actionsMenu.y = topWindow.height;
                    actionsMenu.visible = false;
                }
            }
        }
    }
    onHeightChanged: {
        if(!actionsMenu.visible){
            actionsMenu.y = height;
        }
    }
}
