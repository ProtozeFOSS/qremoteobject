import QtQuick 2.0

Rectangle {
    color: "#e61c1c1c"
    signal initializeSystem()
    property alias keyIdInput:idInput
    Rectangle{
        anchors.centerIn: parent
        width:parent.width*.84
        height:parent.height*.92
        radius:width/8
        color:"white"
        border.color:"grey"
        border.width: 1
        Column{
            anchors.fill: parent
            anchors.margins: parent.radius
            spacing:parent.height*.2
            Text{
                text:"SET KEY FOB"
                font.family: "Lato Black"
                horizontalAlignment: Text.AlignHCenter
                width:parent.width
                font.pixelSize: width/8
                font.bold:true
            }

            Column{
                width:parent.width
                Text{
                    width:parent.width
                    text:"KEY ID"
                    font.pixelSize: width/8
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle{
                    width:parent.width
                    height:60
                    border.width: 1
                    border.color:"black"
                    radius:8
                    TextInput{
                        id:idInput
                        //preeditText:"Key ID"
                        font.capitalization: Font.AllUppercase
                        maximumLength: 4
                        anchors.fill: parent
                        anchors.margins: parent.radius
                        width:parent.width
                        font.pixelSize: height*.7
                        horizontalAlignment: TextInput.AlignHCenter
                    }
                }
            }

            Rectangle{
                color:"#ff8800"
                height:60
                width:parent.width
                radius:8
                border.color: "black"
                border.width:1
                Text{
                    text:"Program Fob"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color:"white"
                    anchors.fill: parent
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        if(idInput.length == 4){
                            initializeSystem();
                        }
                    }
                }
            }
        }
    }
}
