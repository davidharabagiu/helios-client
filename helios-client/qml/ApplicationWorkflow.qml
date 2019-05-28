import QtQuick 2.12
import QtQuick.Controls 2.5
import helios 1.0

StackView {
    id: root

    property var settingsCtl

    signal registrationSuccessful()
    signal notificationAccepted(var notification, var target)
    signal notificationAcceptedDone()

    initialItem: loginScreenComponent

    AuthenticationController {
        id: authCtl

        onLoggedInChanged: {
            if (loggedIn) {
                root.push(mainScreenComponent);
            } else {
                root.pop({item: loginScreenComponent});
            }
        }

        Component.onCompleted: {
            restoreSession();
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
