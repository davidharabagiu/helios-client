import QtQuick 2.12
import helios 1.0

Rectangle {
    id: root

    property bool darkMode: false
    property string username
    property string authToken

    signal logoutButtonActivated

    color: darkMode ? "#000000" : "#ffffff"

    HLabel {
        id: welcomeLabel
        text: "Welcome, " + username
        darkMode: root.darkMode
        anchors {
            left: parent.left
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
    }

    HButton {
        darkMode: root.darkMode
        label: "Log out"
        anchors {
            left: welcomeLabel.right
            leftMargin: 5
            top: parent.top
            topMargin: 5
        }
        onClicked: {
            logoutButtonActivated();
        }
    }

    RemoteFileSystemController {
        id: rfsCtl

        authenticationToken: authToken

        onCwdChanged: {
            fileListing.files = files;
        }
    }

    FileListing {
        id: fileListing
    }

    Rectangle {
        anchors {
            left: parent.left
            top: welcomeLabel.bottom
            leftMargin: 5
            topMargin: 20
            right: parent.right
            rightMargin: 5
            bottom: parent.bottom
            bottomMargin: 5
        }
        clip: true

        ListView {
            anchors.fill: parent
            model: fileListing
            delegate: Rectangle {
                width: 300
                height: 30
                HLabel {
                    darkMode: root.darkMode
                    text: model.fileData.name + " " + (model.fileData.isDirectory ? "[dir]" : model.fileData.size)
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (model.fileData.isDirectory) {
                            rfsCtl.openDirectory(model.fileData.name);
                        }
                    }
                }
            }
        }
    }
}
