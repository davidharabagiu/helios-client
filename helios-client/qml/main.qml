import QtQuick 2.12
import QtQuick.Window 2.12
import helios 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Helios")

    UserAccount {
        username: "pula"
        password: "parolapula"
    }
}
