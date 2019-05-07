import QtQuick 2.12

Rectangle {
    id: root

    property bool darkMode: false

    property string username: usernameInput.text
    property string password: passwordInput.text
    property string passwordRepeated: repeatPasswordInput.text

    color: darkMode ? "#000000" : "#ffffff"

    signal submitButtonActivated()
    signal backButtonActivated()

    Rectangle {
        width: 300
        height: 300
        anchors.centerIn: parent
        color: darkMode ? "#181818" : "#eeeeee"
        radius: 25

        Item {
            id: controlsContainer

            width: usernameInput.width
            height: usernameInput.height + passwordInput.height + repeatPasswordInput.height + registerButton.height + 15
            anchors.centerIn: parent

            HTextInput {
                id: usernameInput
                darkMode: root.darkMode
                anchors {
                    left: parent.left
                    top: parent.top
                }
                width: 165
                hint: "Username"
            }

            HTextInput {
                id: passwordInput
                darkMode: root.darkMode
                password: true
                anchors {
                    left: usernameInput.left
                    top: usernameInput.bottom
                    topMargin: 5
                }
                width: 165
                hint: "Password"
            }

            HTextInput {
                id: repeatPasswordInput
                darkMode: root.darkMode
                password: true
                anchors {
                    left: passwordInput.left
                    top: passwordInput.bottom
                    topMargin: 5
                }
                width: 165
                hint: "Repeat password"
            }

            HButton {
                id: registerButton
                label: "Submit"
                darkMode: root.darkMode
                anchors {
                    left: repeatPasswordInput.left
                    top: repeatPasswordInput.bottom
                    topMargin: 5
                }
                onClicked: {
                    submitButtonActivated();
                }
            }

            HButton {
                id: backButton
                label: "Back"
                darkMode: root.darkMode
                anchors {
                    left: registerButton.right
                    leftMargin: 5
                    top: repeatPasswordInput.bottom
                    topMargin: 5
                }
                onClicked: {
                    backButtonActivated();
                }
            }
        }

        HLabel {
            id: statusLabel
            anchors {
                top: controlsContainer.bottom
                topMargin: 5
                horizontalCenter: controlsContainer.horizontalCenter
            }
            text: ""
        }
    }

    function displayError(message) {
        statusLabel.color = "#ff0000";
        statusLabel.text = message;
    }
}
