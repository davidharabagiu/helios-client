import QtQuick 2.12
import helios 1.0

Rectangle {
    id: root

    property bool darkMode: false
    property string username

    signal logoutButtonActivated

    color: darkMode ? "#000000" : "#ffffff"

    HLabel {
        text: "Welcome, " + username
        darkMode: root.darkMode
        anchors.centerIn: parent
    }

    HButton {
        darkMode: root.darkMode
        label: "Log out"
        anchors {
            left: parent.left
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
        onClicked: {
            logoutButtonActivated();
        }
    }
}
