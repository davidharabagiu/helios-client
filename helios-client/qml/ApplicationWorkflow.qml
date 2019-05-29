import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.2
import helios 1.0

StackView {
    id: root

    property var settingsCtl

    signal registrationSuccessful()
    signal notificationAccepted(var notification, var target)
    signal notificationAcceptedDone()

    initialItem: loginScreenComponent

    Dialog {
        id: restoreSessionDialog
        visible: false
        title: "Restore session"

        contentItem: Rectangle {
            color: settingsCtl.darkMode ? "#000000" : "#ffffff"
            implicitWidth: 250
            implicitHeight: 100

            HTextInput {
                id: restoreSessionPasswordInput

                darkMode: settingsCtl.darkMode
                hint: "Password"
                password: true

                anchors {
                    left: parent.left
                    leftMargin: 5
                    right: parent.right
                    rightMargin: 5
                    top: parent.top
                    topMargin: dialogButtons.y / 2 - height / 2
                }
            }

            Row {
                id: dialogButtons

                anchors {
                    right: parent.right
                    rightMargin: 5
                    bottom: parent.bottom
                    bottomMargin: 5
                    left: parent.left
                    leftMargin: 5
                }

                layoutDirection: Qt.RightToLeft
                height: 25
                spacing: 5

                HButton {
                    label: "Abort"
                    darkMode: settingsCtl.darkMode

                    onClicked: {
                        restoreSessionDialog.visible = false;
                    }
                }

                HButton {
                    label: "Proceed"
                    darkMode: settingsCtl.darkMode

                    onClicked: {
                        authCtl.restoreSession(restoreSessionPasswordInput.text);
                        restoreSessionDialog.visible = false;
                    }
                }
            }
        }
    }

    AuthenticationController {
        id: authCtl

        onLoggedInChanged: {
            if (loggedIn) {
                root.push(mainScreenComponent);
            } else {
                root.pop({item: loginScreenComponent});
            }
        }

        onKeyStorageDecryptionFailed: {
            restoreSessionDialog.visible = true;
        }

        Component.onCompleted: {
            if (hasPersistedLogin) {
                restoreSessionDialog.visible = true;
            }
        }
    }

    Component {
        id: loginScreenComponent

        LoginScreen {
            id: loginScreen

            darkMode: settingsCtl.darkMode

            onRegisterButtonActivated: {
                root.push(registerScreenComponent);
            }

            onLoginButtonActivated: {
                if (!authCtl.login(username, password)) {
                    displayError("Fields can't be empty");
                }
            }

            Binding {
                target: authCtl
                property: "persistLogin"
                value: loginScreen.saveSession
            }

            Connections {
                target: authCtl
                onLoginCompleted: {
                    if (!success) {
                        displayError(errorString);
                    } else {
                        clearInput();
                    }
                }
            }

            Connections {
                target: root
                onRegistrationSuccessful: {
                    displayStatus("All good! You may log in.");
                }
            }
        }
    }

    Component {
        id: registerScreenComponent

        RegisterScreen {
            darkMode: settingsCtl.darkMode

            onBackButtonActivated: {
                root.pop({item: loginScreenComponent});
            }

            onSubmitButtonActivated: {
                if (password !== passwordRepeated) {
                    displayError("Passwords don't match")
                } else if (!authCtl.createUser(username, password)) {
                    displayError("Fields can't be empty");
                }
            }

            Connections {
                target: authCtl
                onUserCreationCompleted: {
                    if (success) {
                        root.registrationSuccessful();
                        root.pop({item: loginScreenComponent});
                    } else {
                        displayError(errorString);
                    }
                }
            }
        }
    }

    Component {
        id: mainScreenComponent

        MainScreen {
            darkMode: settingsCtl.darkMode
            session: authCtl.session

            onLogoutButtonActivated: {
                authCtl.logout();
            }

            onNotificationsButtonActivated: {
                root.push(notificationsScreenComponent);
            }

            onNotificationAccepted: {
                root.notificationAcceptedDone();
            }

            Connections {
                target: root
                onNotificationAccepted: {
                    acceptNotification(notification, target);
                }
            }
        }
    }

    Component {
        id: notificationsScreenComponent

        NotificationsScreen {
            darkMode: settingsCtl.darkMode
            session: authCtl.session

            onBackButtonActivated: {
                root.pop();
            }

            onAcceptButtonActivated: {
                root.notificationAccepted(notification, target);
            }

            Connections {
                target: root
                onNotificationAcceptedDone: {
                    refreshNotifications();
                }
            }
        }
    }
}
