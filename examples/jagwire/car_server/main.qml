import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 546
    height: 243
    title: qsTr("Hello Car")
    Car{
        anchors.fill: parent
    }
}
