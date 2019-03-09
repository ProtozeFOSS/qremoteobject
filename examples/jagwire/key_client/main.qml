import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 448
    height: 732
    title: qsTr("Hello Keyfob")
    color:"#FF282828"
    Keyfob{
        anchors.fill: parent
    }
}
