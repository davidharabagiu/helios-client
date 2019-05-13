import QtQuick 2.12
import QtQuick.Controls 2.5
import helios 1.0

StackView {
    id: root

    property var settingsCtl

    signal registrationSuccessful()

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
            username: authCtl.username
            authToken: authCtl.authenticationToken

            onLogoutButtonActivated: {
                authCtl.logout();
            }
        }
    }
}
