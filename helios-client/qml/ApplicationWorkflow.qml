import QtQuick 2.12
import QtQuick.Controls 2.5
import helios 1.0

StackView {
    id: root

    property var settingsCtl

    initialItem: loginScreen

    Component {
        id: loginScreen

        LoginScreen {
            darkMode: settingsCtl.darkMode

            onRegisterButtonActivated: {
                root.push(registerScreen);
            }
        }
    }

    Component {
        id: registerScreen

        RegisterScreen {
            darkMode: settingsCtl.darkMode

            onBackButtonActivated: {
                root.pop();
            }
        }
    }
}
