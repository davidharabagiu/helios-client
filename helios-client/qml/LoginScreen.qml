import QtQuick 2.12

Rectangle {
    id: root

    property bool darkMode: false

    color: darkMode ? "#000000" : "#ffffff"

    signal loginButtonActivated()
    signal registerButtonActivated()

    Rectangle {
        width: 300
        height: 300
        anchors.centerIn: parent
        color: darkMode ? "#181818" : "#eeeeee"
        radius: 25

        Item {
            width: Math.max(usernameInput.width, Math.max(passwordInput.width, Math.max(saveSessionCheck.width, loginButton.width)))
            height: usernameInput.height + passwordInput.height + saveSessionCheck.height +loginButton.height + 15
            anchors.centerIn: parent

            HTextInput {
                id: usernameInput
                darkMode: root.darkMode
                anchors {
                    left: parent
                    top: parent
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

            HCheckBox {
                id: saveSessionCheck
                darkMode: root.darkMode
                label: "Save session"
                anchors {
                    right: passwordInput.left
                    top: passwordInput.bottom
                    topMargin: 5
                }
            }

            HButton {
                id: loginButton
                label: "Login"
                darkMode: root.darkMode
                anchors {
                    left: saveSessionCheck.left
                    top: saveSessionCheck.bottom
                    topMargin: 5
                }

                onClicked: {
                    loginButtonActivated();
                }
            }

            HButton {
                id: registerButton
                label: "Register"
                darkMode: root.darkMode
                anchors {
                    left: loginButton.right
                    leftMargin: 5
                    top: saveSessionCheck.bottom
                    topMargin: 5
                }

                onClicked: {
                    registerButtonActivated();
                }
            }
        }
    }
}
